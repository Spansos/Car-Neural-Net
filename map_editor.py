import pygame as pg
import sys

class Point:
    def __init__(self, x: int, y: int) -> None:
        self.x = int(x)
        self.y = int(y)
    
    def copy(self):
        return Point(self.x, self.y)

class Line:
    lines = []
    def __init__(self, p1: Point, p2: Point, goal: bool=0) -> None:
        self.p1 = p1
        self.p2 = p2
        self.is_goal = goal
        Line.lines.append(self)
    
    def render(self, surf: pg.Surface, cam: pg.Vector2) -> None:
        if self.is_goal:
            color = (0, 255, 0)
        else:
            color = (255,255,255)
        startpos = (self.p1.x - cam[0], self.p1.y - cam[1])
        endpos = (self.p2.x - cam[0], self.p2.y - cam[1])
        pg.draw.line(surf, color, startpos, endpos)
    
    @classmethod
    def render_all(cls, surf: pg.Surface, cam: pg.Vector2) -> None:
        for i in cls.lines:
            i.render(surf, cam)

def main():
    screen = pg.display.set_mode((800, 600))
    cam = pg.Vector2((0,0))
    goal = False
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
                m_pos2 = get_close_point(m_pos[0], m_pos[1])
                match ev.button:
                    case 1:
                        n_point = m_pos2
                        Line(cur_point, n_point, goal=goal)
                        cur_point = n_point.copy()
                    case 3:
                        cur_point = m_pos2
            if ev.type == pg.KEYDOWN:
                if ev.key == pg.K_p:
                    serialize()
                elif ev.key == pg.K_q:
                    goal = not goal

        keys = pg.key.get_pressed()
        if pg.mouse.get_pressed()[1] or keys[pg.K_e]:
            del_lines = []
            for l in Line.lines:
                if pg.Vector2(l.p1.x,l.p1.y).distance_squared_to(m_pos) < 256 or pg.Vector2(l.p2.x,l.p2.y).distance_squared_to(m_pos) < 256:
                    del_lines.append(l)
            for l in del_lines:
                Line.lines.remove(l)
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

def get_close_point(x, y):
    for l in Line.lines:
        if pg.Vector2(l.p1.x,l.p1.y).distance_squared_to((x,y)) < 256:
            return l.p1.copy()
        elif pg.Vector2(l.p2.x,l.p2.y).distance_squared_to((x,y)) < 256:
            return l.p2.copy()
    return Point(x, y)


def serialize():
    norm_lines = [i for i in Line.lines if not i.is_goal]
    with open('maplines.txt', 'w') as file:
        file.write('')
    with open('maplines.txt', 'a') as file:
        file.write(f"{len(norm_lines)}\n")
        file.writelines([f"{l.p1.x} {l.p1.y} {l.p2.x} {l.p2.y}\n" for l in norm_lines])

    goal_lines = [i for i in Line.lines if i.is_goal]
    with open('goallines.txt', 'w') as file:
        file.write('')
    with open('goallines.txt', 'a') as file:
        file.write(f"{len(goal_lines)}\n")
        file.writelines([f"{l.p1.x} {l.p1.y} {l.p2.x} {l.p2.y}\n" for l in goal_lines])

if __name__ == '__main__':
    main()