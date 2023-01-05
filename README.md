# NoGo
**the term project of Introduction to Computation**

> I consider reading and writing document in English the base skill of a programmer, so I write this document in English.

## About the library
Some of the most common library of the term project includes xege, qt etc. They are matured and easy to use. However, the xege can only be used on Windows and its API are not so elegant. I don't like it. And the qt is a huge library and it takes significantly much time to master it.

So I choose SDL2. But there still a problem. The SDL2 library is designed from c, and I need to write a lot of code in order to use it. So I wrap it into library named ccgame. The ccgame library is a light-weight, high-performance and simple library. It can be used to make small games easily.

## About the features
I don't have time to make the appearance look very fancy, but I take some details into consideration.

I set back button for many page, making the player be able to navigate between pages easily.

To enable player to choose who to play the first chess in single-player mode, I set different function for left click and right click.

To make the chess look more beautiful, I use an anti-aliasing algorithm to draw the circle.

To avoid the player to make small mistakes, I use a algorithm to check if a position can be place by the player. And this algorithm is finished by my partner.

## About the bot
The bot of the easy mode is finished by my partner. It can choose a position with the maximum value to play.

The bot of the hard mode is powered by UCT algorithm. And in order to make it run faster, I refered to KataGo and found out some awesome way.


copyright wangzf2003@stu.pku.edu.cn

