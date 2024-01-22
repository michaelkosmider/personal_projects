import pygame
from random import randint
from ball import Ball
# pygame setup

pygame.init()
screen = pygame.display.set_mode((600, 600))
clock = pygame.time.Clock()
running = True
dt = 0

# create the game elements

player1 = pygame.Rect(20,screen.get_height()//2,5,70)
player2 = pygame.Rect(screen.get_width()-20,screen.get_height()//2,5,70)
turn = -1
ball = pygame.Rect((screen.get_width()//2,screen.get_height()//2),(5,5))
ball_is_in = False
ball_dir = pygame.math.Vector2()

while running:
    # poll for events
    # pygame.QUIT event means the user clicked X to close your window
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # player logic

    keys = pygame.key.get_pressed()
    if keys[pygame.K_w]:
        if player1.y > 0:
            player1.y -= 500 * dt
    if keys[pygame.K_s]:
        if player1.y < screen.get_height()-70:
            player1.y += 500 * dt
    if keys[pygame.K_UP]:
        if player2.y > 0:
            player2.y -= 500 * dt
    if keys[pygame.K_DOWN]:
        if player2.y < screen.get_height()-70:
            player2.y += 500 * dt
    if keys[pygame.K_SPACE]:
        if not ball_is_in:
            ball_is_in = True
            ball_dir = pygame.math.Vector2(turn*5,randint(-5,5)).normalize()
            turn *= -1

    # ball logic
            
    if ball_is_in: 
        ball.y += 300 * ball_dir.y * dt
        ball.x += 300 * ball_dir.x * dt

        # paddle collisions
        if ball.colliderect(player1):
            ball_dir.x *= -1
            ball_dir.y = (ball.center[1]-player1.center[1])/40
            ball_dir.normalize_ip()
            ball.y += 300 * ball_dir.y * dt
            ball.x += 300 * ball_dir.x * dt            
        if ball.colliderect(player2):
            ball_dir.x *= -1
            ball_dir.y = (ball.center[1]-player2.center[1])/40
            ball_dir.normalize_ip()
            ball.y += 300 * ball_dir.y * dt
            ball.x += 300 * ball_dir.x * dt

        #screen edge collisions
        if ball.y <= 0 or ball.y >= screen.get_height():
            ball_dir.y *= -1
        if ball.x <= 0 or ball.x >= screen.get_width():
            ball_is_in = False
            ball.x = screen.get_width()//2
            ball.y = screen.get_height()//2

    # render the new frame
            
    screen.fill("black")
    for i in range(screen.get_height()//20):
        pygame.draw.line(screen, "white", (screen.get_width()//2,i*20),(screen.get_width()//2,i*20+10))
    pygame.draw.rect(screen, "white", player1)
    pygame.draw.rect(screen, "white", player2)
    pygame.draw.circle(screen, "white", ball.center, 5)

    # flip() the display to put your work on screen
    pygame.display.flip()

    # limits FPS to 60
    # dt is delta time in seconds since last frame, used for framerate-
    # independent physics.
    dt = clock.tick(120) / 1000

pygame.quit()