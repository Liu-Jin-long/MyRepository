# 编写于2022年06月14日18时30分46秒
import pygame
from AircraftSprites import *
import time

print(SCREEN_RECT.size)


class AircraftWar(object):
    def __init__(self):
        self.screen = pygame.display.set_mode(SCREEN_RECT.size)
        self.clock = pygame.time.Clock()
        self.__create_sprites()
        # 设置定时器事件和刷新时间
        pygame.time.set_timer(CREATE_ENEMY_EVENT, 1000)
        pygame.time.set_timer(PLAYER_FIRE_EVENT, 300)

    def __create_sprites(self):
        bg1 = BackGround()
        bg2 = BackGround(True)
        self.bg_group = pygame.sprite.Group(bg1, bg2)
        self.enemy_group = pygame.sprite.Group()
        self.player_hero = Hero()
        self.player_group = pygame.sprite.Group(self.player_hero)

    def start_game(self):
        print('游戏开始！')
        while True:
            # 设置刷新频率
            self.clock.tick(FRAME_PER_SEC)
            # 监听各个事件
            self.__event_handler()
            # 碰撞检测
            self.__detect_collision()
            # 更新精灵组
            self.__update_sprites()
            # 更新界面显示
            pygame.display.update()

    def __event_handler(self):
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                AircraftWar.__game_over()
            elif event.type == CREATE_ENEMY_EVENT:
                enemy = EnemyAircraft()
                self.enemy_group.add(enemy)
            elif event.type == PLAYER_FIRE_EVENT:
                self.player_hero.fire()

            keys_pressed = pygame.key.get_pressed()
            if keys_pressed[pygame.K_RIGHT]:
                self.player_hero.speed = 8
            elif keys_pressed[pygame.K_LEFT]:
                self.player_hero.speed = -8
            else:
                self.player_hero.speed = 0

    def __detect_collision(self):
        pygame.sprite.groupcollide(self.player_hero.bullets, self.enemy_group, True, True)
        player_enemies_dict = pygame.sprite.spritecollide(self.player_hero, self.enemy_group, True)
        if len(player_enemies_dict) > 0:
            self.player_hero.kill()
            AircraftWar.__game_over()

    def __update_sprites(self):
        self.bg_group.update()
        self.bg_group.draw(self.screen)

        self.enemy_group.update()
        self.enemy_group.draw(self.screen)

        self.player_group.update()
        self.player_group.draw(self.screen)

        self.player_hero.bullets.update()
        self.player_hero.bullets.draw(self.screen)

    @staticmethod
    def __game_over():
        print('游戏结束！')
        pygame.quit()
        exit()


if __name__ == '__main__':
    pygame.init()
    game = AircraftWar()
    game.start_game()
