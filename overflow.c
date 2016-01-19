#include <stdio.h>
#include <alloca.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>


static void handler(int sig, siginfo_t *si, void *unused) {
  printf("Got SIGSEGV(%d) at address: 0x%lx\n",si->si_code, (long) si->si_addr);
}

void set_signal_handler() {
  static char altstack[SIGSTKSZ];

  stack_t ss = {
    .ss_size = SIGSTKSZ,
    .ss_flags = 0,
    .ss_sp = altstack
  };

  struct sigaction sa = {
    .sa_sigaction = handler,
    .sa_flags = SA_ONSTACK | SA_SIGINFO | SA_RESETHAND
  };

  sigaltstack(&ss, 0);
  sigemptyset(&sa.sa_mask);
  if (sigaction(SIGSEGV, &sa, NULL) == -1) {
    fprintf(stderr, "Test ERROR. Can't set sigaction (%d)\n", errno);
    exit(7);
  }
}

void do_overflow(){
	alloca(sizeof(int));
	do_overflow();
}

int main(char argc, char **argv){
	set_signal_handler();
	do_overflow();
}
