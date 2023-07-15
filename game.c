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
  struct Node *room_node;
};

struct Node {
  struct Room *room;
  struct Node *next;
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

struct Room *find_room_connection(struct Node *room_list, int direction){ 
  //direction: 0-up 1-right 2-bottom 3-left
  struct Node *current_node = room_list;
  while (current_node->next!=NULL){
    if (direction==0 && current_node->room->toproom==NULL){
      return current_node->room;
    } else if (direction==1 && current_node->room->rightroom==NULL){
      return current_node->room;
    } else if (direction==2 && current_node->room->bottomroom==NULL){
      return current_node->room;
    } else if (direction==3 && current_node->room->leftroom==NULL){
      return current_node->room;
    }
    current_node=current_node->next;
  }
  return malloc(sizeof(struct Room));
}

int main() {
  //epic loading screen
  struct winsize size = get_terminal_size();
  int terminal_size=size.ws_col*size.ws_row;
  char screen[terminal_size+1];
  screen[terminal_size]='\0';
  loadingscreen(size,screen);

  //Setup
  int room_counter=1;
  struct Room *current_room = malloc(sizeof(struct Room));
  current_room->roomnumber=room_counter;
  room_counter++;

  struct Node *room_list = malloc(sizeof(struct Node));
  struct Node *end_node = room_list;
  end_node->room=current_room;
  current_room->room_node=end_node;

  char user_move;
  //Movement
  while (1){
    printf("Current Room Number: %d\n",current_room->roomnumber);
    printf("Move (wasd):");
    user_move=getchar();
    fflush(stdin);
    if (user_move=='w'){
      printf("Moving Upwards\n");
      if (current_room->toproom==NULL){
        current_room->toproom=find_room_connection(room_list, 2);
        if (current_room->toproom->roomnumber==0){
          current_room->toproom->roomnumber=room_counter;
          room_counter++;

          end_node->next=malloc(sizeof(struct Node));
          end_node=end_node->next;
          end_node->room=current_room->toproom;
          current_room->toproom->room_node=end_node;
        }
        current_room->toproom->bottomroom=current_room;
      }
      current_room=current_room->toproom;

    } else if (user_move=='s'){
      printf("Moving Downwards\n");
      if (current_room->bottomroom==NULL){
        current_room->bottomroom=find_room_connection(room_list, 0);
        if (current_room->bottomroom->roomnumber==0){
          current_room->bottomroom->roomnumber=room_counter;
          room_counter++;

          end_node->next=malloc(sizeof(struct Node));
          end_node=end_node->next;
          end_node->room=current_room->bottomroom;
          current_room->bottomroom->room_node=end_node;
        }
        current_room->bottomroom->toproom=current_room;
      }
      current_room=current_room->bottomroom;
    } else if (user_move=='a'){
      printf("Moving Left\n");
      if (current_room->leftroom==NULL){
        current_room->leftroom=find_room_connection(room_list, 3);
        if (current_room->leftroom->roomnumber==0){
          current_room->leftroom->roomnumber=room_counter;
          room_counter++;

          end_node->next=malloc(sizeof(struct Node));
          end_node=end_node->next;
          end_node->room=current_room->leftroom;
          current_room->leftroom->room_node=end_node;
        }
        current_room->leftroom->rightroom=current_room;
      }
      current_room=current_room->leftroom;
    } else if (user_move=='d'){
      printf("Moving Right\n");
      if (current_room->rightroom==NULL){
        current_room->rightroom=find_room_connection(room_list, 1);
        if (current_room->rightroom->roomnumber==0){
          current_room->rightroom->roomnumber=room_counter;
          room_counter++;

          end_node->next=malloc(sizeof(struct Node));
          end_node=end_node->next;
          end_node->room=current_room->rightroom;
          current_room->rightroom->room_node=end_node;
        }
        current_room->rightroom->leftroom=current_room;
      }
      current_room=current_room->rightroom;
    } else if (user_move=='p'){
      struct Node *current_node = room_list;
      while (current_node->next!=NULL){
        printf("Room: %d\n",current_node->room->roomnumber);
        if (current_node->room->toproom!=NULL){printf(" ^:%d\n",current_node->room->toproom->roomnumber);};
        if (current_node->room->rightroom!=NULL){printf(" >:%d\n",current_node->room->rightroom->roomnumber);};
        if (current_node->room->bottomroom!=NULL){printf(" V:%d\n",current_node->room->bottomroom->roomnumber);};
        if (current_node->room->leftroom!=NULL){printf(" <:%d\n",current_node->room->leftroom->roomnumber);};
        current_node=current_node->next;
      }
      printf("Room: %d\n",current_node->room->roomnumber);
      if (current_node->room->toproom!=NULL){printf(" ^:%d\n",current_node->room->toproom->roomnumber);};
      if (current_node->room->rightroom!=NULL){printf(" >:%d\n",current_node->room->rightroom->roomnumber);};
      if (current_node->room->bottomroom!=NULL){printf(" V:%d\n",current_node->room->bottomroom->roomnumber);};
      if (current_node->room->leftroom!=NULL){printf(" <:%d\n",current_node->room->leftroom->roomnumber);};
    } else if (user_move=='q'){
      return 0;
    }
  }
};
