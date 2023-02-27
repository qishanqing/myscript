/*
 * remote_setting.h
 *
 *  Created on: Sep 8, 2022
 *      Author: ubuntu
 */

#ifndef INCLUDE_PUBLIC_MIND_OS_REMOTE_REMOTE_SETTING_H_
#define INCLUDE_PUBLIC_MIND_OS_REMOTE_REMOTE_SETTING_H_

namespace mind_os
{

struct RemoteSetting {
    enum Protocol {
        TCP1V1,
        LOCAL_SOCKET
    };

    Protocol protocol;
    std::string ip;
    int port;
    size_t sizeOfCompressLimited = 0;
    std::int32_t timeout = 0;
    int retry = 0;
    std::int64_t interval = 1000; // ms
    int threads = 1;
};

}



#endif /* INCLUDE_PUBLIC_MIND_OS_REMOTE_REMOTE_SETTING_H_ */
