# HSMatchSolver
Finds the best lineups for a best of three games match given a matchup chart. Also provides weighted individual rankings.  
The solution operates under the constraints of Hearthstone's Conquest format with bans.  
Conquest format: https://help.battlefy.com/en/articles/2873888-conquest-format-for-hearthstone-tournaments  
  
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
  
Lineup Rankings (Unweighted): 
1. Big Spell Mage\Holy Paladin\Control Warrior  
Winrate: 0.5545  
2. Thief Rogue\Holy Paladin\Control Warrior  
Winrate: 0.5454  
3. Control Murloc Shaman\Holy Paladin\Control Warrior  
Winrate: 0.5336  
4. Big Spell Mage\Control Murloc Shaman\Control Warrior  
Winrate: 0.5331  
5. Big Spell Mage\Control Murloc Shaman\Holy Paladin  
Winrate: 0.5323  
6. Big Spell Mage\Thief Rogue\Holy Paladin  
Winrate: 0.5199  
7. Control Murloc Shaman\Thief Rogue\Holy Paladin  
Winrate: 0.5154  
8. Fel Demon Hunter\Thief Rogue\Holy Paladin  
Winrate: 0.5116  
9. Big Spell Mage\Thief Rogue\Control Warrior  
Winrate: 0.5096  
10. Fel Demon Hunter\Thief Rogue\Control Warrior  
Winrate: 0.5077  
11. Fel Demon Hunter\Holy Paladin\Control Warrior  
Winrate: 0.5075  
12. Boat Rogue\Holy Paladin\Control Warrior  
Winrate: 0.5075  
13. Control Murloc Shaman\Thief Rogue\Control Warrior  
Winrate: 0.5053  
14. Big Spell Mage\Fel Demon Hunter\Thief Rogue  
Winrate: 0.5013  
15. Big Spell Mage\Fel Demon Hunter\Holy Paladin  
Winrate: 0.4949  
16. Fel Demon Hunter\Control Murloc Shaman\Control Warrior  
Winrate: 0.4942  
17. Big Spell Mage\Fel Demon Hunter\Control Warrior  
Winrate: 0.4940  
18. Big Spell Mage\Fel Demon Hunter\Boat Rogue  
Winrate: 0.4929  
19. Big Spell Mage\Control Murloc Shaman\Thief Rogue  
Winrate: 0.4915  
20. Fel Demon Hunter\Control Murloc Shaman\Holy Paladin  
Winrate: 0.4899  
21. Fel Demon Hunter\Boat Rogue\Control Warrior  
Winrate: 0.4894  
22. Fel Demon Hunter\Boat Rogue\Holy Paladin  
Winrate: 0.4879  
23. Big Spell Mage\Boat Rogue\Holy Paladin  
Winrate: 0.4831  
24. Fel Demon Hunter\Control Murloc Shaman\Boat Rogue  
Winrate: 0.4813  
25. Big Spell Mage\Fel Demon Hunter\Control Murloc Shaman  
Winrate: 0.4792  
26. Fel Demon Hunter\Control Murloc Shaman\Thief Rogue  
Winrate: 0.4791  
27. Control Murloc Shaman\Boat Rogue\Holy Paladin  
Winrate: 0.4788  
28. Control Murloc Shaman\Boat Rogue\Control Warrior  
Winrate: 0.4759  
29. Big Spell Mage\Boat Rogue\Control Warrior  
Winrate: 0.4737  
30. Big Spell Mage\Control Murloc Shaman\Boat Rogue  
Winrate: 0.4521  
  
Individual Rankings (Weighted):  
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
