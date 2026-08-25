We decided not to run a competition again, but still reward good playing strategies with bonus points. Based on the results from the WS 2011, winnig strategies taking 20 rounds or less on average get 5 bonus points. Very good strategies (8 rounds or less) are rewarded with 10 bonus points. In summer term 2012, six students wrote excellent solvers and managed to get 10 bonus points - congratulations!

## WS 2011

In this year's optional competition, the goal was to write a working mastermind client with a good strategy. We had three requirements for clients to be eligible for bonus points:

First, the protocol must not be violated under any circumstances. One unfortunate client managed to perform very well until the last set, where apparently a bug in the solving algorithm caused an assertion to be violated (The 'bad' sequence was **sgbgv**).

Second, no game must last for more than 60 seconds, i.e. the average time for calculating a guess should be at most 2 seconds on average. This did not seem to be a problem for most clients. But again one unfortunate competitor performed well until one of the last tests where the solving algorithm did not terminate within 15 minutes. In this case, the culprit was **sswvo**.

Third, the client needs to have a winning strategy, managing to always guess the secret sequence in at most 35 steps.

If you think that the client you submitted should be ranked in the result list below (performs well, works with reference server, wins game for any of the 8**5 possible secrets), but is not, please send an email to benedikt AT vmars DOT tuwien DOT ac DOT at.

### Evaluation Methodology

We first tried one random sequence for all clients, eliminating those clients which violated the protocol.

For the remaining clients, we first obtained a simple metric _W(c)_ to get a first overview of the quality of the solving algorithms. For this metric, the mastermind always chooses the answer with the most remaining possible secrets.

There was one client with an outstanding performance for this metric, solving the problem in only 5 steps. Indeed, this client also performed best on average, but less significantly so. Here is a plot of _W(c)_ for all eligible clients:

![mastermind w](https://github.com/osue-tuwien/osue-tuwien.github.com/blob/master/images/wiki/mastermind_w.png)

As announced, the actual evaluation took place on a fixed set of secrets. We randomly chose 10 test sets with 100 secret color sequences each, and evaluated the average number _R(t)_ of rounds played for each test set t. Then we calculated the minimum, maximum and average value of _R(t)_ for those 10 test sets. Clients are ranked by the average value of _R(t)_, while the minimum and maximum value is used to cluster clients with similar performance.

### Results

Below is the error bar plot depicting the final result. The x-axis corresponds to the rank r of the client, the y-axis to the average number of rounds per testset. The error bar plot displays the minimum, average and maximum value of R(t), indicating whether there is a significant difference between the performance of two players.

![mastermind results](https://github.com/osue-tuwien/osue-tuwien.github.com/blob/master/images/wiki/mastermind_result.png)

Here are the mastermind players which deserve a **AAA** rating worth 20 bonus points:

<pre>
r MNr     avg(R)  min(R)  max(R)
---------------------------------
1 1027567 5.74753 5.66337 5.81188
2 0919137 5.80297 5.54455 5.91089
3 0828214 5.81881 5.69307 5.9604
4 0926582 5.8604  5.76238 6.0297
5 1027424 5.89307 5.73267 6.0000
</pre>

The next group, rated **AA**, is awarded with 15 bonus points:

<pre>
r MNr     avg(R)  min(R)  max(R)
---------------------------------
6 0929233 6.45842 6.14851 6.60396
7 1026479 6.51584 6.29703 6.86139
8 0774093 6.74654 6.50495 6.9604
9 0927643 6.74951 6.53465 6.85149
</pre>

Next up, the group rated **A**, where all clients manage to win the game in less than 15 steps on average (10 bonus points):

<pre>
r  MNr     avg(R)  min(R)  max(R)
---------------------------------
10 0826568 10.3594 10.1584 10.5446
11 0925911 10.4188 10.2376 10.5743
12 1027929 11.4772 11.3366 11.6931
13 1026412 13.9505 13.8515 14.2574
14 1025519 14.0505 13.8119 14.495
15 1026603 14.3525 14.0297 14.6337
</pre>

Finally, we also decided to award 5 bonus points to an additional group of players, which managed to win the games in less than 20 guesses in each test set.

<pre>
r  MNr     avg(R)  min(R)  max(R)
----------------------------------
16 0925772 15.1881 14.9604 15.4158
17 0928680 16.5713 16.1386 16.9307
18 1026486 16.7307 16.198  17.2475
19 1025723 17.6168 17.0297 18.2178
20 0626177 17.9347 17.703  18.1782
</pre>

Congratulations :)