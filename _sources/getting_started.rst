Getting started
===============

To compile and run the code

.. code-block:: console

    $cd $PATH_TO_PROJECT
    $mkdir build
    $cd build
    $cmake ..

If you wish to compile all files under the ``examples`` folder, execute

.. code-block:: console

    $make

For compiling just one example, execute

.. code-block:: console

    $cmake --build . --target $EXAMPLE_NAME

To run the executable

.. code-block:: console

    $./examples/$EXAMPLE_NAME

   