import pygame as pg
import sys

class Point:
    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y
    
    def copy(self):
        return Point(self.x, self.y)

class Line:
    lines = []
    def __init__(self, p1: Point, p2: Point, col: tuple) -> None:
        self.p1 = p1
        self.p2 = p2
        self.color = col
        Line.lines.append(self)
    
    def render(self, surf: pg.Surface, cam: pg.Vector2) -> None:
        startpos = (self.p1.x - cam[0], self.p1.y - cam[1])
        endpos = (self.p2.x - cam[0], self.p2.y - cam[1])
        pg.draw.line(surf, self.color, startpos, endpos)
    
    @classmethod
    def render_all(cls, surf: pg.Surface, cam: pg.Vector2) -> None:
        for i in cls.lines:
            i.render(surf, cam)

def main():
    screen = pg.display.set_mode((800, 600))
    cam = pg.Vector2((0,0))
    cur_point = Point(0, 0)
    clock = pg.time.Clock()
    while True:
        clock.tick(60)
        m_pos = cam + pg.mouse.get_pos()
        for ev in pg.event.get():
            if ev.type == pg.QUIT:
                pg.quit()
                sys.exit()
            if ev.type == pg.MOUSEBUTTONDOWN:
                match ev.button:
                    case 1:
                        n_point = Point(m_pos.x, m_pos.y)
                        Line(cur_point, n_point, (255, 255, 255))
                        cur_point = n_point.copy()
                    case 3:
                        cur_point.x = m_pos.x
                        cur_point.y = m_pos.y
            if ev.type == pg.KEYDOWN and ev.key == pg.K_p:
                serialize()
        if pg.mouse.get_pressed()[1]:
            del_lines = []
            for l in Line.lines:
                if pg.Vector2(l.p1.x,l.p1.y).distance_squared_to(m_pos) < 256 or pg.Vector2(l.p2.x,l.p2.y).distance_squared_to(m_pos) < 256:
                    del_lines.append(l)
            for l in del_lines:
                Line.lines.remove(l)
        keys = pg.key.get_pressed()
        if keys[pg.K_w]:
            cam.y -= 8
        if keys[pg.K_a]:
            cam.x -= 8
        if keys[pg.K_s]:
            cam.y += 8
        if keys[pg.K_d]:
            cam.x += 8
                        
        
        screen.fill((0,0,0))
        Line.render_all(screen, cam)
        pg.display.update()

def serialize():
    with open('maplines.txt', 'w') as file:
        file.write('')
    with open('maplines.txt', 'a') as file:
        file.write(f"{len(Line.lines)}\n")
        file.writelines([f"{l.p1.x} {l.p1.y} {l.p2.x} {l.p2.y}\n" for l in Line.lines])

if __name__ == '__main__':
    main()