# Motherlode Clone (AKA "Game with sprites")
I learned how to use Directx9 from the book "Beginning Game Programming" by Jonathan S Harbour. Using this new-found knowledge I set out to make a game. I'm not much of an ideas-guy, so I just figured I'd copy someone else's game. There was a popular flash game that I played growing up over on the popular gaming website at the time "addictinggames.com" called "Motherlode" The game was great to me as a kid. I tried my best to recreate it. I have essentially a fully functional game. The graphics are 0/10. This is a prime display of my artistic side. However, the game mechanics are good. I have a fully-fledged upgrade system and everything. Unfortunately the game doesn't really have an ending. Just the bottom of the map and a long pit of nothing.  

# Youtube Playlist
2018-05-18 - 2018-05-25  
This is the youtube playlist of my progress with making the game. You can see improvements as time goes on. I won't detail everything, but I'll make a few points.  
https://www.youtube.com/watch?v=BjnLUYfEBDs&list=PLTBZ3qil-RVUwJTFFmllGWEsbSCOmy8tT&index=1
  
# Video 1
I had some fundamentals with sprites from other test projects that I copied from the book, but I got the sprite to move around with arrow keys.  
# Video 2
I wasn't happy with a simple sprite moving around so I added a ground block. I even included collision detection although you'll see in later videos that more work would yet be needed.  
# Video 3
I generated the world made from 1 object and used a for loop to just keep making that object. This video displays the failures of my collision detection design.  
# Video 4
Added random map generation. It was basically a coin flip if the land would spawn or not. Updated collision detection, but I found more problems.  
# Video 5
Changed the way the camera behaved. It's quite tricky to be honest. You can see that it has a tough time keeping up. I had to read many stack overflow posts to figure out a better way.  
# Video 6
I changed the camera to just follow the player because the camera problem was defeating me. I took this time to add random 'ore' spawns. The lower you go, the more valuable ore that spawns. I was just showing off the entire map in this video.
# Video 7
It's no fun to just fly around so I added the ability for the mining vehicle to actually mine. I intended to add animations, but I never got around to it.
# Video 8
I showed off flaws with my mining function to my friends. Diagonal mining shouldn't have been possible.
# Video 9
I FINALLY added a little HUD with (x,y) positions and now that the vehicle could mine I added an inventory count. I crafted a little refinery in MSPaint to represent the refinery in the game and I added it. There is a trigger for when the player is near it to open a shop menu, but it wasn't really functional. I pulled the button class over from the QuickLinks app that I previously worked on to add buttons to the menu.
# Video 10
I added a currency into the game as well as included a sell function in the refinery. I created a table detailing the value the player would get for each ore.
# Video 11
Now that the player could make money by mining ores, there needs to be a way for the player to spend money. The next thing I added was fuel system and a gas station.
# Video 12
I added an upgrade station for players to spend money at as well. The intention is to buy upgrades to last longer while mining and to mine faster. In this video I was just showing off how in-depth of a menu system I had made was to my friends. Pay no attention to the fact that the upgrade station had the same model as the gas station. I'm not much on making those type of assets.
# Video 13
Just showing off the menu system of the upgrade station.
# Video 14
I finally finished the upgrade station and in this video I demonstrate purchasing random upgrades. I disable the player from selecting a weaker upgrade.
# Video 15
In this video I made the map absurdly large. I don't know how to chunk data or load it in or out at this point so it was all just loaded into memory. (lol)
