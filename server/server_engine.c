#include "server.h"

double acc_vel = 0;
double angle = 0;
double scale_x= 0, scale_y=0;
double Resultante=0;
double Vel_x=0, Vel_y=0;
///Make the ball move smoothly
int StartPosition_x=0, StartPosition_y=0;
int acc_distance;
const double    PI = 3.14159265;


///Players
SDL_Rect rcPlayer1;
SDL_Rect rcPlayer2;
SDL_Rect rcPlayer3;
SDL_Rect rcPlayer4;
///Ball Parameter
SDL_Rect rcball;

void newDirectionBall(double angle, struct SDL_Rect &ball)
{
    acc_distance = 0;
    //printf("\nRCBALL.X: %d ", ball.x);
    //printf("\nRCBALL.Y: %d ", ball.y);
    scale_x = cos((PI*angle)/180);
    scale_y = (sin((PI*angle)/180))*(-1);
    Vel_x = scale_x * acc_vel;
    Vel_y = scale_y * acc_vel;
    StartPosition_x = ball.x;
    StartPosition_y = ball.y;
    Resultante = sqrt((scale_x*scale_x)+(scale_y*scale_y));
    acc_vel += 0.5;
}

void RestartBall(struct SDL_Rect &ball)/// accumulated velocity
{
  acc_vel = 12;
  acc_distance = 0;
  ball.x = SCREEN_WIDTH/2-ball.w/2;
  ball.y = SCREEN_HEIGHT/2-ball.h/2;
}

void resetPlayerPosition(){
  ///Start position Player1
    rcPlayer1.x = SCREEN_WIDTH/2-rcPlayer1.w/2;
    rcPlayer1.y = SCREEN_HEIGHT-50;
    ///Start position Player2
    rcPlayer2.x = SCREEN_WIDTH/2-rcPlayer1.w/2;
    rcPlayer2.y = 25;
    ///Start position Player3
    rcPlayer3.x = 25;
    rcPlayer3.y = SCREEN_HEIGHT/2-rcPlayer1.w/2;
    ///Start position Player4
    rcPlayer4.x = SCREEN_WIDTH-50;
    rcPlayer4.y = SCREEN_HEIGHT/2-rcPlayer1.w/2;
}

void MoveBall(struct SDL_Rect &ball)
{
    ball.x = StartPosition_x + (Vel_x + (Vel_x * acc_distance));
    ball.y = StartPosition_y + (Vel_y + (Vel_y * acc_distance));
    acc_distance++;
}

double angleEffect(struct SDL_Rect ball, struct SDL_Rect player, int playernum)
{
  int ball_x,ball_y;

  if(playernum == 1 || playernum == 2)
  {
    ball_x = ball.x + ball.w/2;

    if(ball_x > (player.x + 50) && ball_x < (player.x + 100)){/// MITTEN 50 pixlar
      return 0;
    }
    else if(ball_x > (player.x + 25) && ball_x < (player.x + 50)){/// vänster mitt
      return -15;
    }
    else if(ball_x > (player.x + 100) && ball_x < (player.x + 125)){/// höger mitt
      return 15;
    }
    else if( ball_x < (player.x + 25) ){ /// längst till vänster
      return -30;
    }
    else if( ball_x > (player.x + 125) ){ ///högersida
      return 30;
    }
  }
  else
  {
    ball_y = ball.y + ball.h/2;

    if(ball_y > (player.y + 50) && ball_y < (player.y + 100)){/// MITTEN 50 pixlar
      return 0;
    }
    else if(ball_y > (player.y + 25) && ball_y < (player.y + 50)){/// vänster mitt
      return 15;
    }
    else if(ball_y > (player.y + 100) && ball_y < (player.y + 125)){/// höger mitt
      return -15;
    }
    else if( ball_y < (player.y + 25) ){ /// längst till vänster
      return 30;
    }
    else if( ball_y > (player.y + 125) ){ ///högersida
      return -30;
    }
  }

  puts("INGEN EFFEKT!");
  return 0;
}

double distance( int x1, int y1, int x2, int y2 ){

	return sqrt( pow(x2-x1,2) + pow(y2-y1,2) );

}


bool Collition(struct SDL_Rect player, struct SDL_Rect ball){
	int px, py,i;
	int ball_x = ball.x + ball.w/2;/// Ball mid X
	int ball_y = ball.y + ball.h/2;/// Ball mid Y
	int ball_r = ball.w/2;
	double dis, dis_y, dis_x;

	// Check closest box x from the ball
	for(i=0;i<ball.w;i++)  
	{
		if( ball_x < (player.x) ){
			px = player.x;
		}
		else if( ball_x > (player.x + player.w) ){
			px = (player.x + player.w);
		}
		else{
			px = ball.x + i;
		}
		//check closest box y from the ball
		if( ball_y < player.y ){
			py = player.y;
		}
		else if( ball_y > (player.y + player.h) ){
			py = (player.y + player.h);
		}
		else{
			py = ball.y + i;
		}
	
		if( distance(ball_x,ball_y,px,py) < ball_r){
			return true;
		}
	}
	return false;
}