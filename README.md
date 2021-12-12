# Monique Mono-Synth

Monique is a monophonic synth from Thomas Arndt which, in December 2021 became open source
as part of the surge-synth-team family of products. We are thrilled Thomas chose to combine 
efforts with the rest of the team.

On of our earliest to-do items as we open source the synth is to write this README properly.

# How to build Monique

Right now we have a suite of open issues as we transition to open source, but we are building using
the same build prescription we use for all our other synths and for Bespoke, namely, once you have forked
and cloned and so on

```bash
cd monique-monosynth
git submodule update --init --recursive
cmake -Bignore/build -DCMAKE_BUILD_TYPE=Debug
cmake --build ignore/build
```


# An important note about licensing

Just like with the rest of our team projects, we welcome any and all contributions from anyone who wants 
to work on open source music software.

At the initial open-sourcing, the copyright to the Monique source was held by Thomas. When open sourcing,
we had several projects - including a potential iOS port - which meant we had to think hard about licensing.

As such, we have decided that Monique should be dual licened GPL3 and MIT. In some contexts, such as the
binaries which you will one day be able to download here for linux, mac, and windows, we are using the GPL3 
version of JUCE, but we may want to also release in situations where open source is still possible, but GPL3 
is not, and then want to use the MIT license.

As a result, we are asking individual contributors who want to contribute to Monique Mono-Synth to sign a CLA.
We have used the Canonical/Harmony 1.0 CLA http://selector.harmonyagreements.org with the following choices:

1. You retain copyright to your code. *We do not need you to assign copyright to us*.
2. You grant us a license to distribute your code under GPL3 or MIT; and your content under CC3 attribution

You can read the entire document [here](MoniqueMonosynth_CLA.pdf).

To agree to this document,
please add your name to the `AUTHORS` list in a git transaction where you indicate in the git log message
that you consent to the CLA. You only need to do this once, and you can do it in your first commit to the repo.
