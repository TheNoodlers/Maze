#!/usr/bin/env python3

import random

class Maze:

   PATH = ' '
   WALL = '#'

   def __init__(self, width, height, complexity = 0.75, density = 0.75):

      # Ensure width and height are odd
      self.width      = (width // 2) * 2 + 1
      self.height     = (height // 2) * 2 + 1
      self.complexity = complexity
      self.density    = density

      self.clear()
      self.boundary()
      self.generate()

   def __str__(self):
      """ convert map to a string """
      text = ''
      for row in self.map:
         for cell in row:
            text += cell
         text += '\n'
      return text[0:-1]

   def build(self, x, y):
      self.map[y][x] = Maze.WALL

   def is_path(self, x, y):
      return self.map[y][x] == Maze.PATH

   def clear(self):
      """ reset to an empty map """
      self.map = []
      for y in range(self.height):
          row = []
          for x in range(self.width):
             row.append(Maze.PATH)
          self.map.append(row)

   def boundary(self):
      """ build boundary wall """

      for x in range(self.width):
         self.build(x, 0)
         self.build(x, self.height - 1)

      for y in range(self.height):
         self.build(0, y)
         self.build(self.width - 1, y)

   def generate(self):
      """ generate a random maze using Prim's algorithm """

      scaled_complexity = 5 * self.complexity * (self.width + self.height)
      scaled_density    = self.density * (self.width // 2) * (self.height // 2)

      for i in range(int(scaled_density)):

         x = random.randint(0, (self.width  // 2) - 1) * 2
         y = random.randint(0, (self.height // 2) - 1) * 2

         self.build(x, y)

         for j in range(int(scaled_complexity)):

            neighbours = []

            if x > 1:
               neighbours.append((x - 2, y))
            if x < (self.width - 2):
               neighbours.append((x + 2, y))
            if y > 1:
               neighbours.append((x, y - 2))
            if y < (self.height - 2):
               neighbours.append((x, y + 2))

            if neighbours != []:

               index  = random.randint(0, len(neighbours) - 1)
               nx, ny = neighbours[index]

               if self.is_path(nx, ny):
                  self.build(nx, ny)
                  self.build(nx + (x - nx) // 2, ny + (y - ny) // 2)
                  x = nx
                  y = ny

maze = Maze(60, 30)

print(maze)

