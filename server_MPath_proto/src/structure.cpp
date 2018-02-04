
 std::shared_ptr<Picture> pict = lf[0]->ReadNextPictureFromVideo();

         std::shared_ptr<Picture> ReadNextPictureFromVideo(void)
        {
            if (m_inputVideoPtr != nullptr)
            {
                return ReadNextPictureFromVideoImpl();
            }
            else
            {
                return nullptr;
            }
        }


        virtual std::shared_ptr<Picture> ReadNextPictureFromVideoImpl(void) override
        {
            auto matptr = m_inputVideoPtr->GetNextPicture(0);   //0 is the streamID
            return std::make_shared<Picture>(matptr->clone());
        }


std::shared_ptr<cv::Mat> VideoReader::GetNextPicture(unsigned streamId)
{
    if (streamId < m_outputFrames.size())
    {
        if (!m_outputFrames[streamId].empty())
        {
            PRINT_DEBUG_VideoReader("Forward next picture for streamId "<<streamId)
            auto matPtr = m_outputFrames[streamId].front();
            m_outputFrames[streamId].pop();
            return matPtr;
        }
        else if (!AllDone(m_doneVect))
        {
            PRINT_DEBUG_VideoReader("Read next few pkt/frames")
            for (auto i = 0; i < 10 && !AllDone(m_doneVect); ++i)
            {
                DecodeNextStep();
            }
            return GetNextPicture( );
        }
    }
    return nullptr;

}



void VideoReader::DecodeNextStep(void)
{
    AVPacket pkt;
    int ret = -1;
    PRINT_DEBUG_VideoReader("Read next pkt")
    if ((ret = av_read_frame(m_fmt_ctx, &pkt)) >= 0) //
    {
        unsigned streamId = pkt.stream_index;
        if (m_streamIdToVecId.count(streamId) > 0 && !m_doneVect[m_streamIdToVecId[streamId]])
        { //then the pkt belong to a stream we care about.
            PRINT_DEBUG_VideoReader("Got a pkt for streamId "<<streamId)
            AVFrame* frame_ptr = av_frame_alloc();
            const AVPacket* const packet_ptr = &pkt;
            bool got_a_frame = false;
            auto* codecCtx = m_fmt_ctx->streams[streamId]->codec;
            PRINT_DEBUG_VideoReader("Send the packet to decoder for streamId "<<streamId)
            ret = avcodec_send_packet(codecCtx, packet_ptr);
            if (ret == 0)
            {//success to send packet
                while(true)
                {
                  PRINT_DEBUG_VideoReader("Ask if a frame is available for streamId " <<streamId)
                  ret = avcodec_receive_frame(codecCtx, frame_ptr);
                  got_a_frame = got_a_frame || (ret == 0);
                  if (ret == 0)
                  {
                      PRINT_DEBUG_VideoReader("Got a frame for streamId " <<streamId)
                      m_gotOne[m_streamIdToVecId[streamId]] = true;
                      //push cv:mat into m_outputFrames[]
                      m_outputFrames[m_streamIdToVecId[streamId]].push(ToMat(codecCtx, frame_ptr));
                      av_frame_unref(frame_ptr);
                  }
                  else
                  {
                    PRINT_DEBUG_VideoReader("No frame available for streamId " <<streamId)
                    break;
                  }
                }
            }
            m_doneVect[m_streamIdToVecId[streamId]] = (m_gotOne[m_streamIdToVecId[streamId]] && (!got_a_frame)) || (m_outputFrames[m_streamIdToVecId[streamId]].size() >= m_nbFrames);
            av_frame_free(&frame_ptr);
        }
        av_packet_unref(&pkt);
        return;
    }
    else
    { //flush all the rest
        PRINT_DEBUG_VideoReader("Start to flush")
        unsigned streamVectId = 0;
        for (auto i = 0; i < m_outputFrames.size(); ++i)
        {
          //send flush signal
          PRINT_DEBUG_VideoReader("Send flush signal for streamId "<<i)
          avcodec_send_packet(m_fmt_ctx->streams[m_videoStreamIds[i]]->codec, nullptr);
        }
        AVFrame* frame_ptr = av_frame_alloc();
        while(!AllDone(m_doneVect))
        {
            if (!m_doneVect[streamVectId])
            {
                bool got_a_frame = false;
                auto* codecCtx = m_fmt_ctx->streams[m_videoStreamIds[streamVectId]]->codec;
                PRINT_DEBUG_VideoReader("Ask for next frame for streamVectId "<<streamVectId)
                int ret = avcodec_receive_frame(m_fmt_ctx->streams[m_videoStreamIds[streamVectId]]->codec, frame_ptr);
                got_a_frame = ret == 0;
                if (got_a_frame)
                {
                    PRINT_DEBUG_VideoReader("Got a frame for streamVectId "<<streamVectId)
                    m_outputFrames[streamVectId].push(ToMat(codecCtx, frame_ptr));
                    av_frame_unref(frame_ptr);
                    //m_outputFrames[streamVectId].emplace();
                }
                m_doneVect[streamVectId] = (!got_a_frame) || (m_outputFrames[streamVectId].size() >= m_nbFrames);
                streamVectId = (streamVectId + 1) % m_outputFrames.size();
            }
        }
        av_frame_free(&frame_ptr);
    }
}



