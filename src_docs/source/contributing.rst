Contributing
############

Anyone is welcome to contribute to this project or create their own variant of it. I would appreciate a PR if your feature would be of benefit other users. 

In order to keep the source code in good condition I use `pre-commit <https://pre-commit.com/>`_ to validate and format the code using their standards for C++/C. 

.. note::  

  If you are using Windows as a base platform I would suggest that you install pre-commit under wsl (Windows Subsystem for Windows) and run it from there, I have found 
  that this approach works fine. 


The following command will run pre-commit on all the source files. Assuming you are in the project directory.

.. code-block:: 

  pre-commit run --files src/*

