/*
 *  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

//
// fbstream code base based upon WEBRTC loopback testing code.
//
// This code is also used as sample code for ViE 3.0
//

// ===================================================================
//
// BEGIN: VideoEngine 3.0 Sample Code
//

#include <iostream>

#include "webrtc/common_types.h"
#include "webrtc/modules/video_coding/codecs/vp8/include/vp8.h"
#include "webrtc/system_wrappers/interface/scoped_ptr.h"
#include "webrtc/test/channel_transport/include/channel_transport.h"
#include "webrtc/video_engine/include/vie_base.h"
#include "webrtc/video_engine/include/vie_capture.h"
#include "webrtc/video_engine/include/vie_codec.h"
#include "webrtc/video_engine/include/vie_external_codec.h"
#include "webrtc/video_engine/include/vie_network.h"
#include "webrtc/video_engine/include/vie_render.h"
#include "webrtc/video_engine/include/vie_rtp_rtcp.h"
#include "webrtc/video_engine/test/auto_test/interface/vie_autotest.h"
#include "webrtc/video_engine/test/auto_test/interface/vie_autotest_defines.h"
#include "webrtc/video_engine/test/libvietest/include/tb_external_transport.h"
#include "webrtc/voice_engine/include/voe_base.h"
#include "webrtc/video_engine/vie_defines.h"
#include "webrtc/video_engine/test/auto_test/interface/vie_svmp_fbstream.h"

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cutils/sockets.h>
#include <android/log.h>
#define VCM_RED_PAYLOAD_TYPE        96
#define VCM_ULPFEC_PAYLOAD_TYPE     97

#undef max
#define max(x,y) ((x) > (y) ? (x) : (y))

#define DEBUG_FBSTREAM 1

struct complete_stream {
	webrtc::VideoEngine* ptrViE;
	webrtc::ViEBase* ptrViEBase;
	webrtc::ViERTP_RTCP* ptrViERtpRtcp;
	webrtc::ViECapture* ptrViECapture;
	webrtc::ViECodec* ptrViECodec;
	webrtc::ViENetwork* ptrViENetwork;
	webrtc::test::VideoChannelTransport* video_channel_transport;
	TbExternalTransport* extTransport;
	int videoChannel;
	int captureId;
	struct svmp_fbstream_init_t init;
	int status;
};

/* strerror_r Wrapper to return a char* instead of int */
char *Strerror_r(int err, char *buf, size_t len) {
		strerror_r(err,buf,len);
		return buf;
}



static int cleanup(struct complete_stream *str){

	int error = 0;

    error = str->ptrViEBase->StopReceive(str->videoChannel);
    if (error == -1)
    {
        printf("ERROR in ViEBase::StopReceive\n");
        return -1;
    }

    error = str->ptrViEBase->StopSend(str->videoChannel);
    if (error == -1)
    {
        printf("ERROR in ViEBase::StopSend\n");
        return -1;
    }

    error = str->ptrViECapture->StopCapture(str->captureId);
    if (error == -1)
    {
        printf("ERROR in ViECapture::StopCapture\n");
        return -1;
    }

    error = str->ptrViECapture->DisconnectCaptureDevice(str->videoChannel);
    if (error == -1)
    {
        printf("ERROR in ViECapture::DisconnectCaptureDevice\n");
        return -1;
    }

    error = str->ptrViECapture->ReleaseCaptureDevice(str->captureId);
    if (error == -1)
    {
        printf("ERROR in ViECapture::ReleaseCaptureDevice\n");
        return -1;
    }

    error = str->ptrViEBase->DeleteChannel(str->videoChannel);
    if (error == -1)
    {
        printf("ERROR in ViEBase::DeleteChannel\n");
        return -1;
    }

    delete str->video_channel_transport;
    delete str->extTransport;

    int remainingInterfaces = 0;
    remainingInterfaces = str->ptrViECodec->Release();
    remainingInterfaces += str->ptrViECapture->Release();
    remainingInterfaces += str->ptrViERtpRtcp->Release();
   // remainingInterfaces += ptrViERender->Release();
    remainingInterfaces += str->ptrViENetwork->Release();
    remainingInterfaces += str->ptrViEBase->Release();
    if (remainingInterfaces > 0)
    {
        printf("ERROR: Could not release all interfaces\n");
        return -1;
    }

    bool deleted = webrtc::VideoEngine::Delete(str->ptrViE);
    if (deleted == false)
    {
        printf("ERROR in VideoEngine::Delete\n");
        return -1;
    }
    return 0;

}

int initVideo(struct complete_stream *str) {

//	printf("!!!\n");
//	printf("DEBUG: Start video streamer to %s:%d\n",str->init.IP,str->init.vidport);
//	printf("!!!\n");
//
//	return 0;

	int error = 0;
	//webrtc::VideoEngine* ptrViE = NULL;
	str->ptrViE = webrtc::VideoEngine::Create();
	if (str->ptrViE == NULL)
	{
		printf("ERROR in VideoEngine::Create\n");
		return -1;
	}

#ifdef DEBUG_FBSTREAM
	error = str->ptrViE->SetTraceFilter(webrtc::kTraceAll);
	if (error == -1)
	{
		printf("ERROR in VideoEngine::SetTraceFilter\n");
		return -1;
	}
#else
	error = str->ptrViE->SetTraceFilter(webrtc::kTraceError);
	if (error == -1)
	{
		printf("ERROR in VideoEngine::SetTraceFilter\n");
		return -1;
	}
#endif

	std::string trace_file = ViETest::GetResultOutputPath() + "fbstream_webrtc_trace.txt";
	error = str->ptrViE->SetTraceFile(trace_file.c_str());
	if (error == -1)
	{
		printf("ERROR in VideoEngine::SetTraceFile\n");
		return -1;
	}

	//
	// Init VideoEngine and create a channel
	//
	//webrtc::ViEBase* ptrViEBase 
	str->ptrViEBase	= webrtc::ViEBase::GetInterface(str->ptrViE);
	if (str->ptrViEBase == NULL)
	{
		printf("ERROR in ViEBase::GetInterface\n");
		return -1;
	}

	error = str->ptrViEBase->Init();
	if (error == -1)
	{
		printf("ERROR in ViEBase::Init\n");
		return -1;
	}

//	webrtc::ViERTP_RTCP* ptrViERtpRtcp =
	str->ptrViERtpRtcp=webrtc::ViERTP_RTCP::GetInterface(str->ptrViE);
	if (str->ptrViERtpRtcp == NULL)
	{
		printf("ERROR in ViERTP_RTCP::GetInterface\n");
		return -1;
	}

	str->videoChannel = -1;
	error = str->ptrViEBase->CreateChannel(str->videoChannel);
	if (error == -1)
	{
		printf("ERROR in ViEBase::CreateChannel\n");
		return -1;
	}

	//
	// List available capture devices, allocate and connect.
	//
	//webrtc::ViECapture* ptrViECapture =
	str->ptrViECapture = webrtc::ViECapture::GetInterface(str->ptrViE);
	if (str->ptrViEBase == NULL)
	{
		printf("ERROR in ViECapture::GetInterface\n");
		return -1;
	}

	const unsigned int KMaxDeviceNameLength = 128;
	const unsigned int KMaxUniqueIdLength = 256;
	char deviceName[KMaxDeviceNameLength];
	memset(deviceName, 0, KMaxDeviceNameLength);
	char uniqueId[KMaxUniqueIdLength];
	memset(uniqueId, 0, KMaxUniqueIdLength);

	printf("Available capture devices:\n");
	int captureIdx = 0;
	for (captureIdx = 0;
			captureIdx < str->ptrViECapture->NumberOfCaptureDevices();
			captureIdx++)
	{
		memset(deviceName, 0, KMaxDeviceNameLength);
		memset(uniqueId, 0, KMaxUniqueIdLength);

		error = str->ptrViECapture->GetCaptureDevice(captureIdx, deviceName,
				KMaxDeviceNameLength, uniqueId,
				KMaxUniqueIdLength);
		if (error == -1)
		{
			printf("ERROR in ViECapture::GetCaptureDevice\n");
			return -1;
		}
		printf("\t %d. %s\n", captureIdx + 1, deviceName);
	}
	printf("\nChoose capture device: ");
#ifdef WEBRTC_ANDROID
	captureIdx = 0;
	printf("0\n");
#else
	if (scanf("%d", &captureIdx) != 1)
	{
		printf("Error in scanf()\n");
		return -1;
	}
	getchar();
	captureIdx = captureIdx - 1; // Compensate for idx start at 1.
#endif
	// Get the name of the device.
	error = str->ptrViECapture->GetCaptureDevice(captureIdx, deviceName,
			KMaxDeviceNameLength, uniqueId,
			KMaxUniqueIdLength);
	if (error == -1)
	{
		printf("ERROR in ViECapture::GetCaptureDevice\n");
		return -1;
	}

	str->captureId = 0;
	error = str->ptrViECapture->AllocateCaptureDevice(uniqueId, KMaxUniqueIdLength,
			str->captureId);
	if (error == -1)
	{
		printf("ERROR in ViECapture::AllocateCaptureDevice\n");
		return -1;
	}

	error = str->ptrViECapture->ConnectCaptureDevice(str->captureId, str->videoChannel);
	if (error == -1)
	{
		printf("ERROR in ViECapture::ConnectCaptureDevice\n");
		return -1;
	}

	//webrtc::VideoCaptureCapability cap;


	error = str->ptrViECapture->StartCapture(str->captureId);
	if (error == -1)
	{
		printf("ERROR in ViECapture::StartCapture\n");
		return -1;
	}

	//
	// RTP/RTCP settings
	//

	error = str->ptrViERtpRtcp->SetRTCPStatus(str->videoChannel,
			webrtc::kRtcpCompound_RFC4585);
	if (error == -1)
	{
		printf("ERROR in ViERTP_RTCP::SetRTCPStatus\n");
		return -1;
	}

	error = str->ptrViERtpRtcp->SetKeyFrameRequestMethod(
			str->videoChannel, webrtc::kViEKeyFrameRequestPliRtcp);
	if (error == -1)
	{
		printf("ERROR in ViERTP_RTCP::SetKeyFrameRequestMethod\n");
		return -1;
	}

	error = str->ptrViERtpRtcp->SetRembStatus(str->videoChannel, true, true);
	if (error == -1)
	{
		printf("ERROR in ViERTP_RTCP::SetTMMBRStatus\n");
		return -1;
	}

	// Setting SSRC manually (arbitrary value), as otherwise we will get a clash
	// (loopback), and a new SSRC will be set, which will reset the receiver.
	error = str->ptrViERtpRtcp->SetLocalSSRC(str->videoChannel, 0x01234567);
	if (error == -1)
	{
		printf("ERROR in ViERTP_RTCP::SetLocalSSRC\n");
		return -1;
	}



	//
	// Setup codecs
	//
	//webrtc::ViECodec* ptrViECodec =
	str->ptrViECodec = webrtc::ViECodec::GetInterface(str->ptrViE);
	if (str->ptrViECodec == NULL)
	{
		printf("ERROR in ViECodec::GetInterface\n");
		return -1;
	}

	// Check available codecs and prepare receive codecs
	printf("\nAvailable codecs:\n");
	webrtc::VideoCodec videoCodec;
	memset(&videoCodec, 0, sizeof(webrtc::VideoCodec));
	int codecIdx = 0;
	for (codecIdx = 0; codecIdx < str->ptrViECodec->NumberOfCodecs(); codecIdx++)
	{
		error = str->ptrViECodec->GetCodec(codecIdx, videoCodec);
		if (error == -1)
		{
			printf("ERROR in ViECodec::GetCodec\n");
			return -1;
		}

		// try to keep the test frame size small when I420
		if (videoCodec.codecType == webrtc::kVideoCodecI420)
		{
			videoCodec.width = 176;
			videoCodec.height = 144;
		}

		error = str->ptrViECodec->SetReceiveCodec(str->videoChannel, videoCodec);
		if (error == -1)
		{
			printf("ERROR in ViECodec::SetReceiveCodec\n");
			return -1;
		}
		if (videoCodec.codecType != webrtc::kVideoCodecRED
				&& videoCodec.codecType != webrtc::kVideoCodecULPFEC)
		{
			printf("\t %d. %s\n", codecIdx + 1, videoCodec.plName);
		}
	}
	printf("%d. VP8 over Generic.\n", str->ptrViECodec->NumberOfCodecs() + 1);

	printf("Choose codec: ");
#ifdef WEBRTC_ANDROID
	codecIdx = 0;
	printf("0\n");
#else
	if (scanf("%d", &codecIdx) != 1)
	{
		printf("Error in scanf()\n");
		return -1;
	}
	getchar();
	codecIdx = codecIdx - 1; // Compensate for idx start at 1.
#endif
	// VP8 over generic transport gets this special one.
	if (codecIdx == str->ptrViECodec->NumberOfCodecs()) {
		for (codecIdx = 0; codecIdx < str->ptrViECodec->NumberOfCodecs(); ++codecIdx) {
			error = str->ptrViECodec->GetCodec(codecIdx, videoCodec);
			assert(error != -1);
			if (videoCodec.codecType == webrtc::kVideoCodecVP8)
				break;
		}
		assert(videoCodec.codecType == webrtc::kVideoCodecVP8);
		videoCodec.codecType = webrtc::kVideoCodecGeneric;

		// Any plName should work with generic
		strcpy(videoCodec.plName, "VP8-GENERIC");
		uint8_t pl_type = 127;
		videoCodec.plType = pl_type;
		webrtc::ViEExternalCodec* external_codec = webrtc::ViEExternalCodec::GetInterface(str->ptrViE);
		assert(external_codec != NULL);
		error = external_codec->RegisterExternalSendCodec(str->videoChannel, pl_type,
				webrtc::VP8Encoder::Create(), false);
		assert(error != -1);
		error = external_codec->RegisterExternalReceiveCodec(str->videoChannel,
				pl_type, webrtc::VP8Decoder::Create(), false);
		assert(error != -1);
	} else {
		error = str->ptrViECodec->GetCodec(codecIdx, videoCodec);
		if (error == -1) {
			printf("ERROR in ViECodec::GetCodec\n");
			return -1;
		}
	}

	//    // Set spatial resolution option
	//    std::string str;
	//    std::cout << std::endl;
	//    std::cout << "Enter frame size option (default is CIF):" << std::endl;
	//    std::cout << "1. QCIF (176X144) " << std::endl;
	//    std::cout << "2. CIF  (352X288) " << std::endl;
	//    std::cout << "3. VGA  (640X480) " << std::endl;
	//    std::cout << "4. 4CIF (704X576) " << std::endl;
	//    std::cout << "5. WHD  (1280X720) " << std::endl;
	//    std::cout << "6. FHD  (1920X1080) " << std::endl;
	//    std::cout << "7. Android  (320X240) " << std::endl;
	//    std::cout << "8. HQ  (480X360) " << std::endl;
	//    std::cout <<"9: VFB (352x288) " << std::endl;

	//std::getline(std::cin, str);
	int resolnOption =  8;// atoi(str.c_str());
	switch (resolnOption)
	{
		case 1:
			videoCodec.width = 176;
			videoCodec.height = 144;
			break;
		case 2:
			videoCodec.width = 352;
			videoCodec.height = 288;
			break;
		case 3:
			videoCodec.width = 640;
			videoCodec.height = 480;
			break;
		case 4:
			videoCodec.width = 704;
			videoCodec.height = 576;
			break;
		case 5:
			videoCodec.width = 1280;
			videoCodec.height = 720;
			break;
		case 6:
			videoCodec.width = 1920;
			videoCodec.height = 1080;
			break;
		case 7:
			videoCodec.width = 320;
			videoCodec.height = 240;
			break;
		case 8:
			videoCodec.width = 480;
			videoCodec.height = 360;
			break;
		case 9:
			videoCodec.width = 352;
			videoCodec.height = 288;
			break;
	}

	// Set number of temporal layers.
	std::cout << std::endl;
	std::cout << "Choose number of temporal layers (1 to 4).";
	std::cout << "Press enter for default: \n";
	//std::getline(std::cin, str);
	// set to 4 temp.
//	int numTemporalLayers = 1; //atoi(str.c_str());
	int numTemporalLayers = 4; //atoi(str.c_str());
	if(numTemporalLayers != 0)
	{
		videoCodec.codecSpecific.VP8.numberOfTemporalLayers = numTemporalLayers;
	}

	// Set start bit rate
	//    std::cout << std::endl;
	//    std::cout << "Choose start rate (in kbps). Press enter for default:  ";
	//std::getline(std::cin, str);
	//int startRate = 0;
	// apyles set to 200
	int startRate= 200;
	if(startRate != 0)
	{
		videoCodec.startBitrate=startRate;

	}

	error = str->ptrViECodec->SetSendCodec(str->videoChannel, videoCodec);
	assert(error != -1);
	error = str->ptrViECodec->SetReceiveCodec(str->videoChannel, videoCodec);
	assert(error != -1);

	//
	// Choose Protection Mode
	//
	//    std::cout << std::endl;
	//    std::cout << "Enter Protection Method:" << std::endl;
	//    std::cout << "0. None" << std::endl;
	//    std::cout << "1. FEC" << std::endl;
	//    std::cout << "2. NACK" << std::endl;
	//    std::cout << "3. NACK+FEC" << std::endl;
	//std::getline(std::cin, str);
	// set default to NONE
	int protectionMethod =  0; //atoi(str.c_str());
	error = 0;
	//bool temporalToggling = true;
	switch (protectionMethod)
	{
		case 0: // None: default is no protection
			break;

		case 1: // FEC only
			error = str->ptrViERtpRtcp->SetFECStatus(str->videoChannel,
					true,
					VCM_RED_PAYLOAD_TYPE,
					VCM_ULPFEC_PAYLOAD_TYPE);
			//temporalToggling = false;
			break;

		case 2: // Nack only
			error = str->ptrViERtpRtcp->SetNACKStatus(str->videoChannel, true);

			break;

		case 3: // Hybrid NAck and FEC
			error = str->ptrViERtpRtcp->SetHybridNACKFECStatus(
					str->videoChannel,
					true,
					VCM_RED_PAYLOAD_TYPE,
					VCM_ULPFEC_PAYLOAD_TYPE);
			//temporalToggling = false;
			break;
	}

	if (error < 0)
	{
		printf("ERROR in ViERTP_RTCP::SetProtectionStatus\n");
	}

	// Set up buffering delay.
	//    std::cout << std::endl;
	//    std::cout << "Set buffering delay (mS). Press enter for default(0mS):  ";
	//std::getline(std::cin, str);
	int buffering_delay = 0; // atoi(str.c_str());
	if (buffering_delay != 0) {
		error = str->ptrViERtpRtcp->SetSenderBufferingMode(str->videoChannel,
				buffering_delay);
		if (error < 0)
			printf("ERROR in ViERTP_RTCP::SetSenderBufferingMode\n");

		error = str->ptrViERtpRtcp->SetReceiverBufferingMode(str->videoChannel,
				buffering_delay);
		if (error < 0)
			printf("ERROR in ViERTP_RTCP::SetReceiverBufferingMode\n");
	}

	//
	// Address settings
	//
	str->ptrViENetwork = webrtc::ViENetwork::GetInterface(str->ptrViE);
	if (str->ptrViENetwork == NULL)
	{
		printf("ERROR in ViENetwork::GetInterface\n");
		return -1;
	}

	// Setup transport.
	//TbExternalTransport* extTransport = NULL;
	//webrtc::test::VideoChannelTransport* video_channel_transport = NULL;

	str->video_channel_transport = new webrtc::test::VideoChannelTransport(str->ptrViENetwork, str->videoChannel);

	//const char* ipAddress = "192.168.42.129";
	//std::cout << "Enter dest. IP: \n";
	//std::string ipAddress;
	//std::getline(std::cin, ipAddress);


	//std::cout << "received IP:" << ipAddress << "\n";
	///std::cout << "streaming to IP:" << argv[1] << "\n";
	std::cout << "streaming to IP:" << str->init.IP << "\n";

	//const unsigned short rtpPort = 5000;


	//std::cout << "Enter dest. Port: \n";
	//std::string tmp_str;
	//std::getline(std::cin, tmp_str);
	//unsigned short rtpPort = atoi(tmp_str.c_str());
	std::cout << std::endl;
	std::cout << "Using rtp port: " << str->init.vidport << std::endl;
	std::cout << std::endl;

	//error = video_channel_transport->SetSendDestination(argv[1], rtpPort);
	error = str->video_channel_transport->SetSendDestination(str->init.IP, str->init.vidport);
	if (error == -1)
	{
		printf("ERROR in SetSendDestination\n");
		return -1;
	}

	error = str->ptrViEBase->StartReceive(str->videoChannel);
	if (error == -1)
	{
		printf("ERROR in ViENetwork::StartReceive\n");
		return -1;
	}

	error = str->ptrViEBase->StartSend(str->videoChannel);
	if (error == -1)
	{
		printf("ERROR in ViENetwork::StartSend\n");
		return -1;
	}



	return 0;
}


static int recvInitPacket(int fd, struct complete_stream *sinfo) {

	int len = sizeof(struct svmp_fbstream_init_t);
	//struct svmp_fbstream_init_t *tmp;
	int tot=0;
	char buf[2*len];
	char errbuf[256];
	memset(buf,0,2*len);
	tot=read(fd,buf,len);
	printf("processing packet init packet: read %d bytes \n",tot);
	if (tot < 0) {
		printf("read error : %s\n",Strerror_r(errno,errbuf,256));
		return tot;
	}
	if (tot == 0){
		printf("read 0 bytes : %s\n",Strerror_r(errno,errbuf,256));
		return tot;
	}

	memcpy(&(sinfo->init),(struct svmp_fbstream_init_t*)buf,len);
	//printf("init packet Gdev:%s, Adev: %s IP: %s, vidport: %d, audport %d, len=%d\n",sinfo->init.Gdev,sinfo->init.Adev,sinfo->init.IP,sinfo->init.vidport,sinfo->init.audport,tot);
	return len;
}



// Initially only support START, STOP functionality
void handlepacket(struct svmp_fbstream_event_t *p, int fd,struct complete_stream *sinfo) {
	int len,err;
	printf("HANDLEPACKET START\n");
	//int tempsdp=0;
        //struct svmp_fbstream_init_t* fbinit = NULL;
	switch (p->cmd) {
		case START: /* read in an initialization packet */
			printf("START\n");
			switch(sinfo->status){
				case STOP:
				case PRINTSDP:
					break;
				default:
					printf("oops already started? sinfo->status:%d\n",sinfo->status);
					return; // only handle START event if PAUSE or STOP
			}
			len = sizeof(struct svmp_fbstream_init_t);
			//fbinit = (struct svmp_fbstream_init_t*)malloc(len);
			memset(&(sinfo->init),0,len);
			err=recvInitPacket(fd,sinfo);
			if (err <= 0 ) {
				printf("error reading init packet\n");
				break;
			}
			printf("calling initVideo\n");
			err=initVideo(sinfo);
			if(err < 0) {
				printf("error initializing Video thread\n");
				break;
			}
			sinfo->status=START;
			break;
		case STOP:
			switch(sinfo->status){
				case START:
				case PLAY:
					break;
				default:
					return; // only handle STOP event if start,or pause
			}
			printf("STOP\n");
			err=cleanup(sinfo);
			if(err < 0) {
				printf("error stopping Video\n");
				break;
			}

			sinfo->status=STOP;
		default:
			printf("unknown packet of type %d received!\n",p->cmd);
			break;
	}
}



static int recvPacket(int fd,struct complete_stream *sinfo) {

	int len = sizeof(struct svmp_fbstream_event_t);
	char buf [len];
	char errbuf[256];
	struct svmp_fbstream_event_t *p = NULL;
	int tot = 0;
	//int tmp;
	memset(buf,0,sizeof(buf));
	printf("processing packet \n");
	tot = read(fd,buf,len);
	printf("processing packet : read %d bytes, sizeof(svmp_fbstream_event_t) = %d \n",tot, sizeof(struct svmp_fbstream_event_t));
	if (tot < 0) {
		printf("read error : %s\n",Strerror_r(errno,errbuf,256));
		return tot;
	}
	if (tot == 0){
		printf("read 0 bytes : %s\n",Strerror_r(errno,errbuf,256));
		return tot;
	}

	p=(struct svmp_fbstream_event_t*)buf;
	printf("packet received cmd :%d, len %d\n",p->cmd,tot);
	handlepacket(p,fd,sinfo);

	return tot;

}


void startListener(struct complete_stream *sinfo) {

	int servfd,newfd,fdmax,
	    i; //,n;
        //socklen_t servlen,clilen;
	fd_set master,read_fds;
	//struct sockaddr_un  cli_addr;//, serv_addr;
	fdmax=0;
	//char buf[80];
	char errbuf[256];
	printf("daemonListenThread\n");

	servfd = android_get_control_socket("fbstr_command");
	if (servfd < 0 )
		printf("error with android_get_control_socket: error %s\n",Strerror_r(errno,errbuf,256));
	else
		printf("android_get_control_socket success fd %d\n", servfd);


	if(listen(servfd, 10) == -1) {
		//pprintf("Could not listen to raw daemon socket");
		printf("listen error %s\n",Strerror_r(errno,errbuf,256));
		return ;
	}

	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	FD_SET(servfd,&master);

	fdmax = max(fdmax,servfd);

	printf("starting select loop\n");
	for (;;) {
		read_fds=master;

		if(select(fdmax+1,&read_fds,NULL,NULL,NULL)==-1)
			printf("select error %s\n",Strerror_r(errno,errbuf,256));

		for(i=0; i<=fdmax; i++){
			if(FD_ISSET(i,&read_fds)) {
				printf("fd is set i=%d, servfd %d \n",i,servfd);
				if(i == servfd) {
					newfd = accept(servfd,NULL,NULL);
					if (newfd >0){
						printf("new connection accepted fd = %d\n",newfd);
						FD_SET(newfd,&master);
						fdmax=max(fdmax,newfd);
					}
					else
						printf("accept error: %s\n", Strerror_r(errno,errbuf,256));
				}else { // existing client
					int n=recvPacket(i,sinfo);
					if (n<=0) {
						printf("closing connection to client \n");
						close(i);
						FD_CLR(i, &master);
					} else {
						// packet processed correctly
						printf("new packet processed size : %d\n",n);
					}
				}
			}
		}
	}

}

//int main2(void* w1, void* w2, int *Argc, char ***Argv)
//{
//	int error = 0;
//	int argc = *Argc;
//	char **argv = *Argv;
//
//	if (argc < 2) {
//		printf("usage: IP, port\n");
//		return -1;
//	}
//
//	const unsigned short rtpPort = atoi(argv[2]);
//	const char* ipAddress = argv[1];
//
//	//
//	// Create a VideoEngine instance
//	//
//	webrtc::VideoEngine* ptrViE = webrtc::VideoEngine::Create();
//	if (ptrViE == NULL)
//	{
//		printf("ERROR in VideoEngine::Create\n");
//		return -1;
//	}
//
//#ifdef DEBUG_FBSTREAM
//	error = ptrViE->SetTraceFilter(webrtc::kTraceAll);
//	if (error == -1)
//	{
//		printf("ERROR in VideoEngine::SetTraceFilter\n");
//		return -1;
//	}
//#else
//	error = ptrViE->SetTraceFilter(webrtc::kTraceError);
//	if (error == -1)
//	{
//		printf("ERROR in VideoEngine::SetTraceFilter\n");
//		return -1;
//	}
//#endif
//
//	std::string trace_file = ViETest::GetResultOutputPath() + "fbstream_webrtc_trace.txt";
//	error = ptrViE->SetTraceFile(trace_file.c_str());
//	if (error == -1)
//	{
//		printf("ERROR in VideoEngine::SetTraceFile\n");
//		return -1;
//	}
//
///* Objects
// * webrtc::VideoEngine* ptrViE;
// * webrtc::ViEBase* ptrViEBase;
// *	webrtc::ViERTP_RTCP* ptrViERtpRtcp;
// * webrtc::ViECapture* ptrViECapture;
// * webrtc::ViECodec* ptrViECodec = webrtc::ViECodec::GetInterface(ptrViE);
// *  webrtc::ViENetwork* ptrViENetwork;
// */
//
//
//	//
//	// Init VideoEngine and create a channel
//	//
//	webrtc::ViEBase* ptrViEBase = webrtc::ViEBase::GetInterface(ptrViE);
//	if (ptrViEBase == NULL)
//	{
//		printf("ERROR in ViEBase::GetInterface\n");
//		return -1;
//	}
//
//	error = ptrViEBase->Init();
//	if (error == -1)
//	{
//		printf("ERROR in ViEBase::Init\n");
//		return -1;
//	}
//
//	webrtc::ViERTP_RTCP* ptrViERtpRtcp =
//			webrtc::ViERTP_RTCP::GetInterface(ptrViE);
//	if (ptrViERtpRtcp == NULL)
//	{
//		printf("ERROR in ViERTP_RTCP::GetInterface\n");
//		return -1;
//	}
//
//
//
//}

//int VideoEngineSampleCode(void* window1, void* window2)
int VideoEngineSampleCode(void* w1, void* w2, int *Argc, char ***Argv)
{
    struct complete_stream str;
    memset(&str, 0, sizeof(struct complete_stream));
    // default state is STOP
    str.status=STOP;
    startListener(&str);
    return 0;
}

