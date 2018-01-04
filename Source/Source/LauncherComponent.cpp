//void LauncherComponent::updateIp() {
//    //Creating a socket
//    int fd = socket(AF_INET, SOCK_DGRAM, 0);
//
//    //This will help us getting the IPv4 associated with wlan0 interface
//    struct ifreq ifr;
//    memset(&ifr, 0, sizeof (ifr));
//    ifr.ifr_addr.sa_family = AF_INET;
//    //Copying the string "wlan0" in the structure
//    sprintf(ifr.ifr_name, "wlan0");
//    //Getting the informations of the socket, so IP address
//    ioctl(fd, SIOCGIFADDR, &ifr);
//    close(fd);
//
//    char* addr = inet_ntoa(((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr);
//    String ip(addr);
//    //Showing the new ip if different than 0.0.0.0
//    if (addr == "0.0.0.0") {
//        labelip.setVisible(false);
//        return;
//    }
//    labelip.setText("IP: " + ip, dontSendNotification);
//    labelip.setVisible(true);
//}
//
//void LauncherComponent::setIpVisible(bool v) {
//    labelip.setVisible(v);
//}




