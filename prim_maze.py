#!/usr/bin/env python3

import random

class Maze:

   def __init__(self, width, height, complexity = 0.75, density = 0.75):
      self.width      = width
      self.height     = height
      self.complexity = complexity
      self.density    = density

      self.generate()

   def print(self):
      """ print map on the console """
      print('+' + ('-' * self.width) + '+')
      for row in self.map:
         print('|', end='')
         for cell in row:
            print(cell, end='')
         print('|')
      print('+' + ('-' * self.width) + '+')

   def clear(self):
      """ reset to an empty map """
      self.map = []
      for y in range(self.height):
          row = []
          for x in range(self.width):
             row.append(' ')
          self.map.append(row)

   def generate(self):
      """ generate a random maze using prims algorithm """

      def build(x, y):
         self.map[y][x] = '#'

      def is_path(x, y):
         return self.map[y][x] == ' '

      self.clear()

      scaled_complexity = 5 * self.complexity * (self.width + self.height)
      scaled_density    = self.density * (self.width // 2) * (self.height // 2)

      for i in range(int(scaled_density)):

         x = random.randint(0, (self.width  // 2) - 1) * 2
         y = random.randint(0, (self.height // 2) - 1) * 2

         build(x, y)

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

               if is_path(nx, ny):
                  build(nx, ny)
                  build(nx + (x - nx) // 2, ny + (y - ny) // 2)
                  x = nx
                  y = ny

maze = Maze(61, 31)

maze.print()

