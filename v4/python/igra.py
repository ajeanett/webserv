#!/usr/bin/env python
# run with python3! (not python2) Ex. /usr/local/bin/python3 igra.py
koloda = [6,7,8,9,10,2,3,4,11] * 4 

import random
random.shuffle(koloda)

print('Let`s play BlackJack?')
count = 0
count2 = 0
final1 = 0
final2 = 0

while True:
    if final1 == 0:
      choice = input('Player 1, Do you want to get a card? y/n\n')
      if choice == 'y':
          current = koloda.pop()
          print('Player 1 got a card with a value %d\n' %current)
          count += current
          if count > 21:
              print('Sorry, but player 1 lost because he has %d points' %count)
              break
          elif count == 21:
              print('Congratulations, you scored 21!')
              final1 = 1
          else:
              print('The first player has% d points.' %count)
      elif choice == 'n':
          print('The first player has% d points.' %count)
          final1 = 1
    if final2 == 0:
      choice2 = input('Player 2, Do you want to get a card? y/n\n')
      if choice2 == 'y':
          current = koloda.pop()
          print('Player 2 got a card with a value %d\n' %current)
          count2 += current
          if count2 > 21:
              print('Sorry, but player 2 lost because he has %d points' %count2)
              break
          elif count2 == 21:
              print('Congratulations, you scored 21!')
              final2 = 1
          else:
              print('The second player has% d points.' %count2)
      elif choice2 == 'n':
          print('The second player has% d points.' %count2)
          final2 = 1

    if final1 == 1 and final2 == 1:
        if count > count2:
          print('Player 1 wins')
        elif count < count2:
          print('Player 2 wins')
        else:
          print('Friendship won')
        break
print('Have a good day!')