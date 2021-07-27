koloda = [6,7,8,9,10,2,3,4,11] * 4

import random
random.shuffle(koloda)

print('Поиграем в BlackJack?')
count = 0
count2 = 0
final1 = 0
final2 = 0

while True:
    if final1 == 0:
      choice = input('Игрок 1, будете брать карту? y/n\n')
      if choice == 'y':
          current = koloda.pop()
          print('Игроку 1 попалась карта достоинством %d\n' %current)
          count += current
          if count > 21:
              print('Извините, но игрок 1 проиграл, т.к. у него %d очков' %count)
              break
          elif count == 21:
              print('Поздравляю, вы набрали 21!')
              final1 = 1
          else:
              print('У игрока 1 %d очков.' %count)
      elif choice == 'n':
          print('У игрока 1 %d очков' %count)
          final1 = 1
    if final2 == 0:
      choice2 = input('Игрок 2, будете брать карту? y/n\n')
      if choice2 == 'y':
          current = koloda.pop()
          print('Игроку 2 попалась карта достоинством %d\n' %current)
          count2 += current
          if count2 > 21:
              print('Извините, но Игрок 2 проиграл, т.к. у него %d очков' %count2)
              break
          elif count2 == 21:
              print('Поздравляю, вы набрали 21!')
              final2 = 1
          else:
              print('У игрока 2 %d очков.' %count2)
      elif choice2 == 'n':
          print('У игрока 2 %d очков' %count2)
          final2 = 1

    if final1 == 1 and final2 == 1:
        if count > count2:
          print('Победил игрок 1')
        elif count < count2:
          print('победил игрок 2')
        else:
          print('Победила дружба')
        break
print('До новых встреч!')