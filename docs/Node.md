### node
node executes itself when someone calls it (another node, loop or user from patch editor).

when executes node works with input data (upper pins) and produces output data (lower pins), and can call following nodes. by default node call following node on pin 0 (default trigger pin).

### trigger
![](Link%20types.png)

trigger link is used to indicate what next node to call. under the hood its a data link but with no data.
