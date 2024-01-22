from turtle import Turtle

class Midline(Turtle):

    def __init__(self,length):
        super().__init__()
        self.length = length
        self.color('white')

    def draw_line(self):
        self.penup()
        self.goto(0,self.length//2-5)
        self.setheading(270)
        for _ in range(self.length//40):
            self.pendown()
            self.fd(20)
            self.penup()
            self.fd(20)
        self.hideturtle()
