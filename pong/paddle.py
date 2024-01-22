from turtle import Turtle

class Paddle(Turtle):

    def __init__(self,x,y):
        super().__init__()
        self.shape('square')
        self.color('white')
        self.penup()
        self.goto(x,y)
        self.shapesize(3,0.1)

    def move_up(self):
        self.goto(self.xcor(),self.ycor()+10)

    def move_down(self):
        self.goto(self.xcor(),self.ycor()-10)