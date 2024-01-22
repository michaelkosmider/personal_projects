from turtle import Screen,Turtle
from midline import Midline
from paddle import Paddle
from time import sleep

#configuring the screen

SCREEN_SIZE = 600
screen = Screen()
screen.setup(width=SCREEN_SIZE,height=SCREEN_SIZE)
screen.bgcolor('black')
screen.tracer(0)

#drawing the pong midline

midline = Midline(SCREEN_SIZE)
midline.draw_line()
screen.update()

#initializing the paddles

left_paddle = Paddle(-280,0)
right_paddle = Paddle(280,0)
screen.update()

#initialize key listeners

screen.listen()
screen.onkeypress(left_paddle.move_down,'s')
screen.onkeypress(left_paddle.move_up,'w')
screen.onkeypress(right_paddle.move_down,'Down')
screen.onkeypress(right_paddle.move_up,'Up')

#start the main game loop

game_on = True
while game_on:
    screen.update()
    # sleep(0.1)

screen.exitonclick()