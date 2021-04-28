#ifndef __TCP_SEND_H__
#define __TCP_SEND_H__

#include <string>

namespace tcp_send {

class TCPClient {
   public:
    TCPClient(const std::string& serverIp, int serverPort);
    ~TCPClient();
    int SendStrViaTCP(const std::string& strSent, std::string* strReturn);
    int getBufferSize() const;
    void setBufferSize(int bfsz);

    std::string serverIp_;
    int serverPort_;

   private:
    char serverRet_[256];
    int bufferSize_ = 256;
    int clientFd_;
};

}  // namespace tcp_send

#endif
