void poller(int socketIn, std::string srcIP, int port){
 struct pollfd fds[2];
 // Initialize the fds to 0
 memset(fds, 0, sizeof(fds));

 // Wait in ms before poll times out. -1 for infinite
 int timeout = 15000;

 fds[0].fd = socketIn;

 while(true){
  fds[0].events = POLLIN;
  fds[1].fd = socketOut;
  fds[1].events = POLLIN;

  if(inWritePending){
   fds[1].events = POLLOUT;
   fds[0].events = 0;
  }

  if(outWritePending){
   fds[0].events = POLLOUT;
   if(!inWritePending){
    fds[1].events = 0;
   }
  }

  /*
  Poll is a unix function. POSIX specification has select and poll
  More systems support select than poll
  Returns -1 if an error occurred
  Returns 0 if no descriptors are ready before the timer expires
  Otherwise returns the number of descriptors that have nonzero revents members

  POLLIN - Data is ready to recv() on this socket
  POLLOUT - Can send() to this socket without blocking
  POLLERR - Error on socket
  POLLHUP - Remote connection hanged up
  POLLNVAL - Error with socket descriptor
  */

  if(poll(fds, 2, timeout) > 0){
   if(fds[0].revents & (POLLNVAL|POLLERR|POLLHUP)){
    clientdisconnected();
    break;
   }
   if(fds[1].revents & (POLLNVAL|POLLERR|POLLHUP)){
    serverdisconnected();
    break;
   }
   if(fds[0].revents & POLLIN){
    clientread();
    break;
   }

   if(fds[1].fd!=0 && fds[1].revents & POLLOUT){
    serverwrite();
   }

   if(fds[1].fd!=0 && fds[1].revents & POLLIN){
    serverread();
   }

   if(fds[0].revents & POLLOUT){
    clientwrite();
    break;
   }

  }
 closeconn();
}
