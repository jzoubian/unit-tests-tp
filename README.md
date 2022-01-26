Unit test practical session
===========================

Connecrt to the toy server:
---------------------------

ssh -o StrictHostKeyChecking=no -o "UserKnownHostsFile /dev/null"   USER@134.158.21.51 -L8080:localhost:8080
ssh -o StrictHostKeyChecking=no -o "UserKnownHostsFile /dev/null"   USER@134.158.21.53 -L8080:localhost:8080
ssh -o StrictHostKeyChecking=no -o "UserKnownHostsFile /dev/null"   USER@134.158.21.57 -L8080:localhost:8080

Clone the sources
-----------------

```sh
git clone /home/admin/sebv/unit-test-tp.git
```

Debugging MALT
--------------

Extract /home/admin/sebv/part-1-malt-bug.tar.bz2 in a directory.

Go into the `part-1-malt-bug/malt` directory and build malt.

```sh
cd part-1-malt-bug/malt
mkdir build
cd build
../configure --enable-debug --prefix=$HOME/usr
make
make install
```

If you want to play with it:

```sh
# profile the application
malt ./tests/test-main
# launch the webview
# The first time it will ask the create a user/password to secure the GUI
malt-webview -i malt-test-main-89762.json
```

```sh
# we forward the port to get access to the webview from
# our browser
ssh server -L8080:localhost:8080
```

Now you can open your local browser on http://localhost:8080:
 - Go in the source view
 - Click on function `call_a` on the left
 - Look on the page bottom table
 - It says the allocated memory is 0

You can launch an integation test:

```sh
ctest
```

Move to verbose mode to get its output;

```sh
ctest -V
```

Try to debug like this. Not so easy if you don't know the code in advance (even like this). 
Take a few minutes to try anyway.

After a few minutes, lets enable the unit tests:

```sh
./configure --enable-debug --enable-tests
make
make test
```

Find the faulty unit test;

You can run only this one with:

```sh
ctest -R {TEST_NAME} -V
```

**TODO**: Find the source location of the test and go to it.
**TODO**: Look on what is used and go in the implementation source (you might need to use grep to find the function).
**TODO**: Look on the code and find the issue.

Run again the tests to check the solution.
Is it not easier with unit tests ?

First unit test in python
-------------------------

We will make out first unit test, in python.
