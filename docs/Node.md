### node
node executes itself in corresponding thread when someone calls it (another node, loop or user from patch editor).

![](Do%20times.png)

when executes node works with input data (upper pins) and produces output data (lower pins), and can call following nodes. by default node call following node on pin 0 (default trigger pin).

### trigger
![](Link%20types.png)

trigger link is used to indicate what next node to call. color of trigger pins and link corresponds to actual thread. under the hood its a data link but with no data.

in this example order of nodes calls is following: 

**number_io** (sets 0 on data link) -> **sleep** (stops thread for n seconds (default is 1)) -> **print_number** (prints number from number_io)
