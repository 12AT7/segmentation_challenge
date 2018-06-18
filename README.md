# Segmentation Programming Challenge

This is a take home computer vision exercise that we are using to evaluate
candidates for fitting into our Algorithms team.  The challenge is deliberately
open ended, to minimize our preconceptions about the technical backgrounds of
engineers that might succeed with us.  To solve the challenge, you must choose
a specific topic that fits your technical background, and show us something
interesting that you investigated using your own self direction.

## Requirements:

We are looking for broad technical competence useful for a computer vision and
high performance computing software team.  There are many aspects to this, and
nobody knows everything about every topic.  The challenge is designed for you
to select a specific skillset that you are great at, and use it to show us what
you can do.  We have just a few requirements:

* The project must be written in C++.  Python would be way easier if all we
  wanted to see was a solved algorithm, but actually we are just as interested
in your ability to engineer total software systems as your ability to innovate
a novel algorithm.  If you want to make plots or other numerical analysis for
any reason, you may use Python or other suitable language for that purpose, but
keep the main computation in C++.

* To get the skeleton and your solution to compile, you will have to show
  enough Linux chops (using the distribution of your choice) to get some
dependencies installed, invoke a sufficiently modern C++ compiler, link, and
run.  The minimum dependencies are Boost, Blaze (a linear algebra library), and
LibPNG.

* Return the results to us by cloning this repository, configuring a new
  remote into your own Github or other online service, and committing your
changes there.  Please do not fork, because that will allow other candidates to
see who you are by looking at Github.

## Suitable Topics

Here, we enumerate some interesting topics to investigate.  These are all great
ideas, so you get to choose what subset you want to do, based on your interest, time, and
skillset.

### Algorithms

For showing us an algorithm, you can choose to either use existing open source
C++ libraries (of your choice) to implement a larger computation, or else write
smaller computations (such as connected-component labeling) from scratch.  We
do prefer to see modern C++ patterns rather than old ones (like OpenCV) but
both can get the job done.

* Detect shapes in the synthetic mask image using any standard computer vision algorithm.
* Compute morphological feature vectors from the segmented shapes, and classify into
  categories such as `square` and `circle` using an algorithm of your choice.
* Add more shapes until they overlap; can you detect and classify colliding shapes as neither squares nor circles?
* Investigate the performance and degradations of the algorithm as the noise and shape density are varied.
* Any other related quantitative investigation that you find interesting and shows off your strengths

### High Performance Computing

If you would prefer to show how you might take an existing algorithm (that you
may or may not understand well) and make it efficient, well tested, easier to
read, or any other quality improvements that you are good at and find
interesting, then (if you want it) we will provide additional code for you to
start with.  (This additional code would spoil the above Algorithms topics,
which is why it is an addendum.)
* Parallelize, using `std::thread` or `std::async` and friends, one or more
  computations.  We do not expect an actual speedup on such a small toy
problem, but are instead looking at style, correctness, and knowledge of
asyncronous programming and standard libraries.
* Clever ideas to achieve high test coverage, which is challenging for numerical software
* Integrate the project on CircleCI or other continuous integration service
* Vary the image size and object density, and measure the timing of your
  calculation, to demonstrate scaling performance.  If you make the images
large enough, you should be able to detect your computer's cache size.
* Any other related high performance computing topic that you find interesting and shows off your strengths

## Workflow Demonstration

This section describes some results that we will notice about your workflow, if
you provide them.  There is no way you will get to show all of these outputs in
a limited time.  The idea is for you to choose the ones that show off your
strengths, and not be disadvantaged by getting stuck on random details outside
of your education and experience.

* Well styled and disciplined Git log that demonstrate how you might
  incrementally build software and organize pull requests with your team
* CMake configuration to find the dependencies, compile, and link
* Unit tests - use a framework of your choice (such as Boost Test, Google Test, Catch, or Mettle).  Given the limited time, providing just a few tests in a workflow is almost as good as acheiving high coverage (which takes much longer).
* Use commonly available engineering tools to prove that the software lacks memory leaks, race conditions (if using threads), and/or identify performance bottlenecks (profiling).
* Critique of the framework we provided, bug fixes, feature enhancements, or performance optimizations.
* If you end up writing other images, plots, or profiling results that helped
  you debug or develop the project, we will look at those too to further
understand what you did.

## Questions and Hints

We want to see your strengths, and not get stuck on issues that do not
contribute to your main investigation.  If have questions, or have found a bug
or problem with our challenge, then please reach out to get it resolved or
worked around.  Getting blocked on unintended snags does not help either of us
evaluate your skills.

Furthermore, if you just want some help over an early hurdle so you can focus
on something else, reach out for that too.  We can provide things like
`CMakeLists.txt`, or simple algorithms, to help you get engaged on some
specific topic more quickly.
