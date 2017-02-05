# add auto bulb with text boxes for time and number of exposures

from __future__ import division
from __future__ import print_function

import pygame
import socket

pygame.init()   # Start Pygame
done = False    # Loop until the user clicks the close button.

# UDP Constants
TARGET_IP = "192.168.1.51"
#TARGET_IP = "173.250.159.234"
UDP_PORT = 8888
MAX_BUFFER_SIZE = 24
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
message = ""

# Define some colors
WHITE = (255, 255, 255)
GOLD = (213, 202, 148)
RED = (225, 40, 40)
GREEN = (0, 120, 5)
BLACK = (0, 0, 0)
# Button colors
SENDCOLOR = RED
TEXTCOLOR = GOLD
POTCOLOR = RED
SHUTTERCOLOR = RED

# Define initial characteristics of the screen
size = (700, 500)
screen = pygame.display.set_mode(size)
screen.fill(WHITE)
SENDBUT = pygame.Rect(20, 20, 225, 200)
DISPLAY = pygame.Rect(20, 350, 400, 100)
SHUTTERBUT = pygame.Rect(475, 20, 200, 200)
POT = pygame.Rect(340, 20, 125, 200)
pygame.display.set_caption("Camera Remote")
font = pygame.font.SysFont('Arial', 25)

clock = pygame.time.Clock()   # Used to manage how fast the screen updates

# Method to redraw the default, commonly used screen
def redraw_screen():
    screen.fill(WHITE)
    sendButton = pygame.draw.rect(screen, SENDCOLOR, SENDBUT, 0)
    pygame.draw.rect(screen, SHUTTERCOLOR, SHUTTERBUT, 0)
    pot = pygame.draw.rect(screen, POTCOLOR, POT, 0)
    display = pygame.draw.rect(screen, GREEN, DISPLAY, 0)
    pygame.draw.rect(screen, GREEN, DISPLAY, 0)
    shutter = pygame.draw.rect(screen, SHUTTERCOLOR, SHUTTERBUT, 0)
    pygame.draw.rect(screen, POTCOLOR, POT, 0)
    logo = pygame.image.load("dslr.png")
    logorect = logo.get_rect()
    logorect.centerx = 575
    logorect.centery = 400
    screen.blit(logo, logorect)
    text = font.render('Shutter', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = shutter.centerx
    textpos.centery = shutter.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Pot Stop', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = pot.centerx
    textpos.centery = pot.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    text = font.render('Send Commands', 1, TEXTCOLOR)
    textpos = text.get_rect()
    textpos.centerx = sendButton.centerx
    textpos.centery = sendButton.centery
    screen.blit(text, (textpos.centerx - textpos.width/2, textpos.centery - textpos.height/2))
    pygame.display.flip()

# UDP initialization stuff
if __name__ == '__main__':
    print("UDP Port: ", UDP_PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(('0.0.0.0', UDP_PORT))
    sock.settimeout(0.01)

# ------ Main Program Loop -------
while not done:
    redraw_screen()

    # --- Main event loop
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            done = True
        elif event.type == pygame.MOUSEBUTTONDOWN:
            click = SENDBUT.collidepoint(pygame.mouse.get_pos())
            if click == 1:
                if SENDCOLOR == RED:
                    SENDCOLOR = GREEN
                else:
                    SENDCOLOR = RED
            click = SHUTTERBUT.collidepoint(pygame.mouse.get_pos())
            if click == 1:
                if SHUTTERCOLOR == RED:
                    SHUTTERCOLOR = BLACK
                else:
                    SHUTTERCOLOR = RED
            click = POT.collidepoint(pygame.mouse.get_pos())
            if click == 1:
                if POTCOLOR == RED:
                    POTCOLOR = BLACK
                else:
                    POTCOLOR = RED

    pygame.display.flip() # -- update screen

    clock.tick(120) # -- limit to 120 frames per second

    pygame.event.pump() # Get and convert joystick value, print

    if SENDCOLOR == GREEN: # send UDP stuff
        pot_flag = 1
        if POTCOLOR == BLACK:
            pot_flag = 0
        stop_flag = 1
        if SHUTTERCOLOR == BLACK:
            stop_flag = 0
        message = 'Sending: '.join([chr(pot_flag), chr(stop_flag)])
        print(message)
        sock.sendto(message, (TARGET_IP, UDP_PORT))
        pygame.time.wait(10)

pygame.quit()