# 编写于2022年06月14日18时31分52秒
import pygame
import random

# 屏幕分辨率
SCREEN_RECT = pygame.Rect(0, 0, 480, 700)
# 刷新帧速率
FRAME_PER_SEC = 15
#创建敌机的定时器常量，为事件定义不同名字的常量
CREATE_ENEMY_EVENT = pygame.USEREVENT
#玩家发射子弹事件
PLAYER_FIRE_EVENT = pygame.USEREVENT + 1


class GameSprite(pygame.sprite.Sprite):
    def __init__(self, image_name, speed=1):
        super(GameSprite, self).__init__()
        self.image = pygame.image.load(image_name)
        self.rect = self.image.get_rect()
        self.speed = speed

    def update(self):
        self.rect.y += self.speed


class BackGround(GameSprite):
    def __init__(self, another=False):
        super(BackGround, self).__init__('./images/background.png')
        if another:
            self.rect.y = -self.rect.height


class EnemyAircraft(GameSprite):
    def __init__(self):
        super(EnemyAircraft, self).__init__('./images/enemy1.png')
        self.speed = random.randint(3, 8)
        self.rect.bottom = 0
        x_range = SCREEN_RECT.width - self.rect.width
        self.rect.x = random.randint(0, x_range)

    def update(self):
        super().update()
        if self.rect.y >= SCREEN_RECT.height:
            self.kill()

    def __del__(self):
        # print('敌机销毁')
        pass


class Hero(GameSprite):
    def __init__(self):
        # 设置玩家战机禁止竖直移动
        super(Hero, self).__init__('./images/me1.png', 0)
        # 设置用户战机对齐界面x轴中央以及距离界面底部距离
        self.rect.centerx = SCREEN_RECT.centerx
        self.rect.bottom = SCREEN_RECT.bottom - 20
        self.bullets = pygame.sprite.Group()

    def update(self):
        self.rect.x += self.speed
        if self.rect.x < 0:
            self.rect.x = 0
        elif self.rect.right > SCREEN_RECT.right:
            self.rect.right = SCREEN_RECT.right

    def fire(self):
        print('发射三连发子弹')
        for i in range(3):
            bullet = Bullet()

            bullet.rect.centerx = self.rect.centerx
            bullet.rect.bottom = self.rect.top - i * 14
            self.bullets.add(bullet)

class Bullet(GameSprite):
    def __init__(self):
        # 注意设置子弹方向和速度
        super(Bullet, self).__init__('./images/bullet2.png', -8)

    def update(self):
        super().update()
        if self.rect.bottom < 0:
            self.kill()

    def __del__(self):
        print('子弹销毁')
        pass
