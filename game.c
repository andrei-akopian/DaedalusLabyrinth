#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <time.h>

struct Room {
  int roomnumber;
  struct Room *toproom;
  struct Room *bottomroom;
  struct Room *rightroom;
  struct Room *leftroom;
};

struct winsize get_terminal_size() {
  struct winsize size;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &size) == -1) {
      perror("ioctl");
      return size;
  }
  return size;
}

char *generateLoadingScreenFrame(int p, struct winsize size, char *screen){
  //make background
  char filler_text[]="pointer       ";
  int j=0;
  int i=0;
  for (j=0;j<size.ws_row;j++){
    p=p-j;
    for(i=0;i<size.ws_col;i++){
      screen[j*size.ws_col+i]=filler_text[p%strlen(filler_text)];
      p++;
    };
  }
  //make game title
  char title[]="Daedalus Labyrinth";
  int anchor=size.ws_row/2*size.ws_col+size.ws_col/2-strlen(title)/2;
  screen[anchor-size.ws_col-1]='#';
  screen[anchor-1]='#';
  screen[anchor+size.ws_col-1]='#';
  for (i=0;i<strlen(title);i++){
    screen[anchor-size.ws_col+i]='#';
    screen[anchor+i]=title[i];
    screen[anchor+size.ws_col+i]='#';
  }
  screen[anchor-size.ws_col+i]='#';
  screen[anchor+i]='#';
  screen[anchor+size.ws_col+i]='#';
  return screen;
}
void loadingscreen(struct winsize size,char *screen){
    int p=0;
    for(int i=0;i<20;i++){
      screen=generateLoadingScreenFrame(p,size,screen);
      printf("\n%s",screen);
      fflush(stdout);
      usleep(100000);
      system("clear");
      p++;
    }
};


int main() {
  //epic loading screen
  struct winsize size = get_terminal_size();
  int terminal_size=size.ws_col*size.ws_row;
  char screen[terminal_size+1];
  screen[terminal_size]='\0';
  loadingscreen(size,screen);
  //actuall game
  int room_counter=0;
  struct Room *current_room = malloc(sizeof(struct Room));
  current_room->roomnumber=room_counter;
  room_counter++;
  char user_move;
  while (1){
    printf("Current Room Number: %d\n",current_room->roomnumber);
    printf("Move (wasd):");
    user_move=getchar();
    fflush(stdin);
    if (user_move=='w'){
      printf("Moving Upwards\n");
      if (current_room->toproom==NULL){
        current_room->toproom=malloc(sizeof(struct Room));
        current_room->toproom->roomnumber=room_counter;
        current_room->toproom->bottomroom=current_room;
        room_counter++;
      }
      current_room=current_room->toproom;
    } else if (user_move=='s'){
      printf("Moving Downwards\n");
      if (current_room->bottomroom==NULL){
        current_room->bottomroom=malloc(sizeof(struct Room));
        current_room->bottomroom->roomnumber=room_counter;
        current_room->bottomroom->toproom=current_room;
        room_counter++;
      }
      current_room=current_room->bottomroom;
    } else if (user_move=='a'){
      printf("Moving Left\n");
      if (current_room->leftroom==NULL){
        current_room->leftroom=malloc(sizeof(struct Room));
        current_room->leftroom->roomnumber=room_counter;
        current_room->leftroom->rightroom=current_room;
        room_counter++;
      }
      current_room=current_room->leftroom;
    } else if (user_move=='d'){
      printf("Moving Right\n");
      if (current_room->rightroom==NULL){
        current_room->rightroom=malloc(sizeof(struct Room));
        current_room->rightroom->roomnumber=room_counter;
        current_room->rightroom->leftroom=current_room;
        room_counter++;
      }
      current_room=current_room->rightroom;
    }
  }
};
