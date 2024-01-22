from turtle import Turtle, Screen

win = Screen()

flynn = Turtle('turtle')

def process_events():
    events = tuple(sorted(key_events))

    if events and events in key_event_handlers:
        (key_event_handlers[events])()

    key_events.clear()

    win.ontimer(process_events, 200)

def Up():
    key_events.add('UP')

def Down():
    key_events.add('DOWN')

def Left():
    key_events.add('LEFT')

def Right():
    key_events.add('RIGHT')

def move_up():
    flynn.setheading(90)
    flynn.forward(25)

def move_down():
    flynn.setheading(270)
    flynn.forward(20)

def move_left():
    flynn.setheading(180)
    flynn.forward(20)

def move_right():
    flynn.setheading(0)
    flynn.forward(20)

def move_up_right():
    flynn.setheading(45)
    flynn.forward(20)

def move_down_right():
    flynn.setheading(-45)
    flynn.forward(20)

def move_up_left():
    flynn.setheading(135)
    flynn.forward(20)

def move_down_left():
    flynn.setheading(225)
    flynn.forward(20)

key_event_handlers = { \
    ('UP',): move_up, \
    ('DOWN',): move_down, \
    ('LEFT',): move_left, \
    ('RIGHT',): move_right, \
    ('RIGHT', 'UP'): move_up_right, \
    ('DOWN', 'RIGHT'): move_down_right, \
    ('LEFT', 'UP'): move_up_left, \
    ('DOWN', 'LEFT'): move_down_left, \
}

key_events = set()

win.onkey(Up, "Up")
win.onkey(Down, "Down")
win.onkey(Left, "Left")
win.onkey(Right, "Right")

win.listen()

process_events()

win.mainloop()