import pygame as pg

class Point:
    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

class Line:
    lines = []
    def __init__(self, p1: Point, p2: Point, col: tuple) -> None:
        self.p1 = p1
        self.p2 = p2
        self.color = col
        Line.lines.append(self)
    
    def render(self, surf: pg.Surface) -> None:
        pg.draw.line(surf, self.color, (self.p1.x, self.p1.y), (self.p2.x, self.p2.y))
    
    @classmethod
    def render_all(cls, surf: pg.Surface) -> None:
        for i in cls.lines:
            i.render(surf)

def main():
    pass

if __name__ == '__main__':
    main()