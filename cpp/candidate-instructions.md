# Introduction

This test is an extremely simplified and rather contrived example of what a risk system in a typical bank might look like. For the purposes of simplicity of this test, market and static data as well as presentation (UI & UX) have been completely ignored.

For the purposes of this test, imagine that this is a new system that was originally developed only to handle government bonds and is now being extended to handle corporate bonds and FX. As a result the system needs to be enhanced.

All of the below exercises should be performed as though you are working on a real risk system. **You should feel free to refactor any code as you see fit and you should always attempt to reuse as much code as possible and write the minimum amount of new code.**

Apart from the exercises where you are asked to write or modify unit tests, none of the other unit tests should be modified.

There are a number of occasions in the test where multiple solutions are possible. You should decide how it is most appropriate to proceed and be prepared to justify your decisions and assumptions.

At the end of the exercise all of the code should compile, the unit tests should run successfully, and the program should produce the expected output.

**As this is a C++ test, you should also identify and fix any C++ specific issues you find in the codebase, such as memory management problems, inefficient parameter passing, or resource leaks.**

# Rules

This test should take about 2 hours to complete and you should try and limit your time to this. You should allocate your time accordingly and skip any questions you are unable to complete. The questions get progressively more difficult, the first few questions should take only a matter of minutes to complete, the later questions will take longer. Depending on your experience you may not be able to complete all of the questions. This is deliberate and expected.

You may use any resources you wish to complete the test. You may use the internet, books, or any other resources you have available including AI tools. You should not ask for help from any other person. **You will be asked to explain your solution and justify your decisions.**

You should not use any third party libraries or introduce any new dependencies in order to complete the test. You may use any functionality provided by the C++ standard library (C++17 or later). The default CMake configuration uses C++17, but you may change `CMAKE_CXX_STANDARD` in CMakeLists.txt to 20 or 23 if you prefer.

Start by forking this repository to your own GitHub account. You should commit your code after each exercise at a minimum. Once you have completed the test, please send the URL of your repository to your interviewer.

# Building

```bash
mkdir build
cd build
cmake ..
make
```

# Running

### Console Application
```bash
cd build
./ConsoleApp
```

### Tests
```bash
cd build
./Tests
```

# Exercises

## Exercise 1
Modify the [BondTradeLoader](./Loaders/BondTradeLoader.cpp) class to correctly process the bond feed file ([BondTrades.dat](./Loaders/TradeData/BondTrades.dat)) such that it now creates the correct objects for both government and corporate bonds. Once this is complete the BondTradeLoaderTests should all pass.

## Exercise 2
Implement the [FxTradeLoader](./Loaders/FxTradeLoader.cpp) class so that it can read the FX trades feed file ([FxTrades.dat](./Loaders/TradeData/FxTrades.dat)). The Instrument field should be constructed as the concatenation of Ccy1 and Ccy2. Once this is implemented the FxTradeLoaderTests should all pass.

## Exercise 3
Implement the [PricingConfigLoader](./RiskSystem/PricingConfigLoader.cpp) class such that it loads the contents of the [PricingEngines.xml](./RiskSystem/PricingConfig/PricingEngines.xml) file and provides the `PricingEngineConfig` as a vector of `PricingEngineConfigItem` objects. Once this is implemented the PricingConfigLoaderTests should all pass.

## Exercise 4
Implement the `loadPricers` function in the [SerialPricer](./RiskSystem/SerialPricer.cpp) class so that it instantiates each of the required types of pricing engines based on the configuration.

## Exercise 5
Complete the implementation of the [ScalarResults](./Models/ScalarResults.h) class such that the iterator interface is fully implemented for range-based for loops. Once this is complete the ScalarResultsTests should all pass.

## Exercise 6
Implement the [ScreenResultPrinter](./RiskSystem/ScreenResultPrinter.cpp) class. It should output one line to the screen for each trade in the following formats:

- If there is both a result and an error: `TradeID : Result : Error`
- If there is a result but no error: `TradeID : Result`
- If there is an error but no result: `TradeID : Error`

At this point you should be able to run the application ConsoleApp. It should load all of the trades and print one line per trade with results output.

## Exercise 7
The risk system in its state as provided loads all of the trades from all trade sources before attempting to start to price any of them. For a high volume system this could present serious memory problems. 

Implement the [StreamingTradeLoader](./RiskSystem/StreamingTradeLoader.h) class (with implementation in [StreamingTradeLoader.cpp](./RiskSystem/StreamingTradeLoader.cpp)) such that it will load a trade and immediately send it to be priced so that the entire trade population does not have to be held in memory. You may need to modify the `ITradeLoader` interface or create a new streaming interface. It is acceptable to have all of the results still held in memory for the purposes of this test.

## Exercise 8
The system in its current state will serialise the pricing of each trade one at a time. This does not scale well in a system with complex trade types that take a long time to price. 

Implement the [ParallelPricer](./RiskSystem/ParallelPricer.h) class (with implementation in [ParallelPricer.cpp](./RiskSystem/ParallelPricer.cpp)) that will use threads to run a number of pricing calculations in parallel. The call to the `price` function should still only return once all of the results are available.