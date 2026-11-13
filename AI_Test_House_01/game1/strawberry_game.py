
import pygame
import random

WIDTH = 800
HEIGHT = 600

score = 0
time_left = 30
game_over = False

fall_speed = 220
move_speed = 500

player_image = pygame.transform.smoothscale(
    images.girl, (100, 130)
)

strawberry_image = pygame.transform.smoothscale(
    images.strawberry, (45, 50)
)

player_rect = player_image.get_rect()
player_rect.midbottom = (400, 580)

strawberry_rect = strawberry_image.get_rect()


def reset_strawberry():
    strawberry_rect.midtop = (
        random.randint(25, WIDTH - 25), 0
    )


reset_strawberry()


def draw():
    screen.fill((255, 225, 238))

    screen.blit(player_image, player_rect)
    screen.blit(strawberry_image, strawberry_rect)

    screen.draw.text(
        "YUM YUM STRAWBERRY!",
        (220, 15),
        fontsize=32,
        color="black"
    )

    screen.draw.text(
        "SCORE: " + str(score),
        (25, 65),
        fontsize=30,
        color="black"
    )

    screen.draw.text(
        "TIME: " + str(int(time_left)),
        (25, 105),
        fontsize=30,
        color="black"
    )

    if game_over:
        screen.draw.text(
            "GAME OVER!",
            center=(400, 270),
            fontsize=65,
            color="red"
        )

        screen.draw.text(
            "FINAL SCORE: " + str(score),
            center=(400, 340),
            fontsize=35,
            color="black"
        )

        screen.draw.text(
            "PRESS R TO RESTART",
            center=(400, 400),
            fontsize=25,
            color="black"
        )


def update(dt):
    global score, time_left, game_over

    if game_over:
        return

    time_left -= dt

    if time_left <= 0:
        time_left = 0
        game_over = True
        return

    if keyboard.left:
        player_rect.x -= move_speed * dt

    if keyboard.right:
        player_rect.x += move_speed * dt

    if player_rect.left < 0:
        player_rect.left = 0

    if player_rect.right > WIDTH:
        player_rect.right = WIDTH

    strawberry_rect.y += fall_speed * dt

    if player_rect.colliderect(strawberry_rect):
        score += 1
        reset_strawberry()

    elif strawberry_rect.top > HEIGHT:
        reset_strawberry()


def on_key_down(key):
    global score, time_left, game_over

    if game_over and key == keys.R:
        score = 0
        time_left = 30
        game_over = False

        player_rect.midbottom = (400, 580)

        reset_strawberry()
