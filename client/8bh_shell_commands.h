#pragma once

  typedef int (*CmdFn)(char* cmd, int fd, char* buf);
  typedef struct {
    const char* name;
    int num_args;
    CmdFn fn;
    unsigned char cod;
    const char* help;
  } Command;

  extern Command commands[];

  extern const int num_commands;

  int initFn(int fd, char* buf);

  Command* findCommand(const char* name);

  int executeCommand(char* name);

  int workFn(char* cmd, int fd, char* buf);

  void dimmer(int ret, int fd, char* buf_dim);