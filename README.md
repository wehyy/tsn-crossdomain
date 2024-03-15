# Deterministic Cross-Domain Flow Scheduling (DCDFS)
DCDFS uses OMNeT++ discrete event simulation framework to provide a simulation model for Time Sensitive Networking (TSN), named tsn-crossdomain.
Our model uses the [INET framework](https://inet.omnetpp.org/) and an open source extension CoRE4INET.

## Compatibility

The current version of the tsn-crossdomain simulation framework has been tested with OMNeT++ version 5.5.1 and INET version 3.3.6 under Windows.

## Getting Started

+ Follow the instructions at [https://omnetpp.org/](https://omnetpp.org/) to download and install OMNeT++ version 5.5.1
+ Make sure OMNeT++ is in your `PATH` by changing into the OMNeT++ directory and sourcing the `setenv` script.
+ `cd` into an arbitrarily named, preferably empty `<workspace>` directory.
+ Clone this repository, i.e.

```
  $ git clone https://github.com/wehyy/tsn-crossdomain.git
```

+ Download and unpack INET version 3.3.6 or clone the INET repository and checkout tag `v3.3.6`, i.e.

```
  $ git clone https://github.com/inet-framework/inet.git
  $ git checkout -b v3.6.6 v3.6.6
```
  or alternatively with a space-saving one-liner:
```
  $ git clone --branch v3.6.6 --depth 1 https://github.com/inet-framework/inet.git
```

+ Download and unpack CoRE4INET or clone the CoRE4INET repository, i.e.

```
  $ git clone https://github.com/CoRE-RG/CoRE4INET.git
```

+ Your directory should now look like this:

```
  <workspace>
  ├── tsn-crossdomain
  └── inet
  └── CoRE4INET
```

### Importing and building from the OMNeT++ IDE

+ Start the OMNeT++ IDE, either from your desktop environment or by calling `omnetpp` from the terminal.
+ Set your previously chosen `<workspace>` directory as the workspace and launch the IDE.
+ Import both `tsn-crossdomain` and `inet` and `CoRE4INET` into your workspace:
	- Select `File -> Import...` from the menu.
	- Select `General -> Existing Projects into Workspace` and click `Next >`.
	- Under `Select root directory:`, choose the `<workspace>` directory.
	- Both project folders should now appear and be checked under `Projects`.
	- Click `Finish` and wait for the indexer to complete.
+ Build both INET, CoRE4INET and tsn-corssdomain by right-clicking on the corresponding project folder and selecting `Build Project`.
  (You can switch between the `release` and `debug` configuration in the same context menu under `Build Configurations -> Set Active`)
+ You can now run the supplied example simulation:
	- Navigate into `tsn-crossdomain > simulations` in the project explorer.
	- Right-click `omnetpp.ini` and choose `Run As` (or `Debug As` depending on the build configuration) `OMNeT++ Simulation`.
	- After a possible build step you will be presented with a graphical interface showing the simulation model.

### Building from the terminal

If you want to use tsn-crossdomain without the IDE or using OMNeT++ core, you can also build INET and NeSTiNg and run simulations from the terminal. (By default, the release versions of both projects will be built. To build the debug version, call `MODE=debug make` instead of `make` below. tsn-crossdomain requires the INET library and CoRE4INET to be built with the same configuration.)

+ To build INET:

```
  $ cd inet
  $ make makefiles
  $ make
```

+ To build CoRE4INET:

```
  $ cd ../CoRE4INET
  $ make makefiles
  $ make
```

+ To build tsn-crossdomain (both the library and the simulation executable):

```
  $ cd ../tsn-crossdomain
  $ make makefiles
  $ make
```

+ To run the example simulation, change to the `tsn-crossdomain/simulations/cross_domain_demo2` directory and call one of the following

```
  $ ./runsim omnetpp.ini                # run simulation without graphical interface (release)
  $ ./runsim-qt omnetpp.ini             # run simulation with the Qt interface (release)
  $ MODE=debug ./runsim omnetpp.ini     # run simulation without graphical interface (debug)
  $ MODE=debug ./runsim-qt omnetpp.ini  # run simulation with the Qt interface (debug)
```

## More Information

The paper [Deterministic Cognition: Cross-Domain Flow Scheduling for Time-Sensitive Networks](https://ieeexplore.ieee.org/abstract/document/10422798/) has been accepted by IEEE Trans. Cogn. Commun. Netw., 2024.
