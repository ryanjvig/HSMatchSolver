# HSMatchSolver
Finds the best lineups for best of 3 and best of 5 game matches given a matchup chart. Also provides weighted individual rankings.  
Solution is made using Markov chains and operates under the constraints of Hearthstone's Conquest format with bans.  
Conquest format: https://help.battlefy.com/en/articles/2873888-conquest-format-for-hearthstone-tournaments  

Command line format:  
s < [INPUT FILE] > [OUTPUT FILE]

Input file format:  
[# of Decks]  
[Deck 1 Name], [Deck 1 Weight] [Deck 1 Class] [Deck 1 Matchups]  
...  
[Deck n Name], [Deck n Weight] [Deck n Class] [Deck n Matchups]  
  
Example Input:  
7  
Big Spell Mage, .2 m .50 .35 .60 .55 .45 .65 .60  
Fel Demon Hunter, .2 d .65 .50 .55 .40 .45 .30 .45  
Control Murloc Shaman, .1 s .40 .45 .50 .55 .60 .45 .45  
Thief Rogue, .2 r .45 .60 .45 .50 .55 .40 .50  
Boat Rogue, .15 r .55 .55 .40 .45 .50 .35 .45  
Holy Paladin, .05 p .35 .70 .55 .60 .65 .50 .40  
Control Warrior, .1 w .40 .55 .55 .50 .55 .60 .50  
  
Example Output:  
  
Best of 5 Lineup Rankings (Unweighted):
1. Big Spell Mage\Control Murloc Shaman\Holy Paladin\Control Warrior
Winrate: 0.5433
2. Big Spell Mage\Thief Rogue\Holy Paladin\Control Warrior
Winrate: 0.5401
3. Control Murloc Shaman\Thief Rogue\Holy Paladin\Control Warrior
Winrate: 0.5252
4. Fel Demon Hunter\Thief Rogue\Holy Paladin\Control Warrior
Winrate: 0.5214
5. Big Spell Mage\Control Murloc Shaman\Thief Rogue\Holy Paladin
Winrate: 0.5196
6. Big Spell Mage\Fel Demon Hunter\Thief Rogue\Holy Paladin
Winrate: 0.5152
7. Fel Demon Hunter\Control Murloc Shaman\Thief Rogue\Holy Paladin
Winrate: 0.5097
8. Big Spell Mage\Control Murloc Shaman\Thief Rogue\Control Warrior
Winrate: 0.5092
9. Big Spell Mage\Fel Demon Hunter\Holy Paladin\Control Warrior
Winrate: 0.5045
10. Big Spell Mage\Boat Rogue\Holy Paladin\Control Warrior
Winrate: 0.5040
11. Fel Demon Hunter\Control Murloc Shaman\Thief Rogue\Control Warrior
Winrate: 0.5034
12. Fel Demon Hunter\Boat Rogue\Holy Paladin\Control Warrior
Winrate: 0.5026
13. Big Spell Mage\Fel Demon Hunter\Boat Rogue\Holy Paladin
Winrate: 0.5023
14. Big Spell Mage\Fel Demon Hunter\Thief Rogue\Control Warrior
Winrate: 0.5006
15. Fel Demon Hunter\Control Murloc Shaman\Boat Rogue\Holy Paladin
Winrate: 0.4959
16. Big Spell Mage\Fel Demon Hunter\Boat Rogue\Control Warrior
Winrate: 0.4935
17. Big Spell Mage\Fel Demon Hunter\Control Murloc Shaman\Holy Paladin
Winrate: 0.4933
18. Fel Demon Hunter\Control Murloc Shaman\Holy Paladin\Control Warrior
Winrate: 0.4923
19. Control Murloc Shaman\Boat Rogue\Holy Paladin\Control Warrior
Winrate: 0.4892
20. Big Spell Mage\Control Murloc Shaman\Boat Rogue\Holy Paladin
Winrate: 0.4891
21. Big Spell Mage\Fel Demon Hunter\Control Murloc Shaman\Thief Rogue
Winrate: 0.4879
22. Fel Demon Hunter\Control Murloc Shaman\Boat Rogue\Control Warrior
Winrate: 0.4852
23. Big Spell Mage\Fel Demon Hunter\Control Murloc Shaman\Control Warrior
Winrate: 0.4839
24. Big Spell Mage\Fel Demon Hunter\Control Murloc Shaman\Boat Rogue
Winrate: 0.4827
25. Big Spell Mage\Control Murloc Shaman\Boat Rogue\Control Warrior
Winrate: 0.4775

Best of 3 Lineup Rankings (Unweighted):
1. Big Spell Mage\Holy Paladin\Control Warrior
Winrate: 0.5577
2. Thief Rogue\Holy Paladin\Control Warrior
Winrate: 0.5366
3. Big Spell Mage\Control Murloc Shaman\Control Warrior
Winrate: 0.5321
4. Control Murloc Shaman\Holy Paladin\Control Warrior
Winrate: 0.5320
5. Big Spell Mage\Control Murloc Shaman\Holy Paladin
Winrate: 0.5298
6. Control Murloc Shaman\Thief Rogue\Holy Paladin
Winrate: 0.5136
7. Big Spell Mage\Thief Rogue\Control Warrior
Winrate: 0.5102
8. Fel Demon Hunter\Thief Rogue\Control Warrior
Winrate: 0.5088
9. Fel Demon Hunter\Thief Rogue\Holy Paladin
Winrate: 0.5083
10. Big Spell Mage\Thief Rogue\Holy Paladin
Winrate: 0.5079
11. Control Murloc Shaman\Thief Rogue\Control Warrior
Winrate: 0.5053
12. Fel Demon Hunter\Holy Paladin\Control Warrior
Winrate: 0.5039
13. Big Spell Mage\Fel Demon Hunter\Thief Rogue
Winrate: 0.5017
14. Boat Rogue\Holy Paladin\Control Warrior
Winrate: 0.5000
15. Big Spell Mage\Fel Demon Hunter\Holy Paladin
Winrate: 0.4982
16. Fel Demon Hunter\Control Murloc Shaman\Control Warrior
Winrate: 0.4981
17. Big Spell Mage\Fel Demon Hunter\Control Warrior
Winrate: 0.4952
18. Big Spell Mage\Fel Demon Hunter\Boat Rogue
Winrate: 0.4946
19. Big Spell Mage\Control Murloc Shaman\Thief Rogue
Winrate: 0.4944
20. Fel Demon Hunter\Control Murloc Shaman\Holy Paladin
Winrate: 0.4933
21. Fel Demon Hunter\Boat Rogue\Control Warrior
Winrate: 0.4925
22. Fel Demon Hunter\Boat Rogue\Holy Paladin
Winrate: 0.4894
23. Control Murloc Shaman\Boat Rogue\Holy Paladin
Winrate: 0.4817
24. Fel Demon Hunter\Control Murloc Shaman\Thief Rogue
Winrate: 0.4807
25. Control Murloc Shaman\Boat Rogue\Control Warrior
Winrate: 0.4801
26. Big Spell Mage\Fel Demon Hunter\Control Murloc Shaman
Winrate: 0.4798
27. Fel Demon Hunter\Control Murloc Shaman\Boat Rogue
Winrate: 0.4751
28. Big Spell Mage\Boat Rogue\Control Warrior
Winrate: 0.4727
29. Big Spell Mage\Boat Rogue\Holy Paladin
Winrate: 0.4700
30. Big Spell Mage\Control Murloc Shaman\Boat Rogue
Winrate: 0.4565

Individual Deck Rankings (Weighted):
1. Holy Paladin
Winrate: 0.5475
2. Control Warrior
Winrate: 0.5075
3. Thief Rogue
Winrate: 0.5075
4. Big Spell Mage
Winrate: 0.5000
5. Fel Demon Hunter
Winrate: 0.4925
6. Control Murloc Shaman
Winrate: 0.4875
7. Boat Rogue
Winrate: 0.4875
