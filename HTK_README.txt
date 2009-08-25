Licence
=======
simon uses the closed source HTK to create and modify the speech model.

The licence of HTK is not compatible with the GPL.

However, simon never links to the HTK but rather executes them as separate
executable which means that this incompatibility has no effect.


Open alternatives
=================
simons architecture is designed so that the HTK is easily replacable. All
calls to HTK software are contained in one single class:
ModelCompilationManager
(simonlib/speechmodelcompilation/modelcompilationmanager.cpp).

At the moment there are, as far as I know, no valid free and open source
replacements.

However, as soon as a valid, free replacement presents itself, we, the simon
team, would be very interested in replacing the HTK with it.


Difference to the "Java trap"
============================
Even without the HTK, simon will be a very useful tool. You can still collect
training data, design vocabularies or even use existing speech models for
recognition.

The ASCII format of models created with the HTK is well documented and there
are converters from SPHINXs format to the HTK format so that you can set up a
working simon system without ever using the HTK.
