## 1. Creating local environment on the server
Run the following command to create a local environment to run the tests (on the csl server):\
<b>&nbsp;&nbsp;&nbsp;&nbsp;$ source <(curl -s https://bit.ly/3poR8nU) <path-to-your-eventManager.py></b>

<b>Example of how I would do it on the server</b>\
&nbsp;&nbsp;&nbsp;&nbsp;$ cd /home/saar.ofek/mtm/ex02/\
&nbsp;&nbsp;&nbsp;&nbsp;$ source <(curl -L -s https://bit.ly/3poR8nU) ./eventManager.py


## 2. Running the tests after step 1
Step 1 will create a directory called "saars_tests" in the current directory you are at (where you did cd to).

<b>Now run:</b>\
&nbsp;&nbsp;&nbsp;&nbsp;$ cd saars_tests/\
&nbsp;&nbsp;&nbsp;&nbsp;$ python3.6 ./run_tests.py

This will run the tests locally on the server.


(Don't copy the "$" in the commands, run them without it)