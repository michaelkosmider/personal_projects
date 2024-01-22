from pygame import Rect

class Ball(Rect):

    def __init__(self,x,y,w,h):
        super().__init__(x,y,w,h)
        self.is_in = False
        self.dir = (1,0)

    def launch(self):
        self.is_in = True
