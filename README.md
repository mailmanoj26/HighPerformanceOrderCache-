# HighPerformanceOrderCache-
A high-performance C++17 in-memory order cache and matching engine for financial securities. Designed to efficiently handle up to 1 million orders with optimized data structures and sequential matching logic.

# Key Features
- Performance Optimized: Processes 1M+ orders within strict compute limits using unordered_map-based caching.
- Order Management: Add, cancel, and query orders by ID, user, or security.
- Matching Engine: Implements realistic buy/sell matching rules, preventing intra-company matches.
- Robust Validation: Input validation ensures integrity (no duplicates, invalid sides, or zero quantities).
- Comprehensive Testing: Google Test suite with functional, edge-case, and performance benchmarks.
- Cross-Platform: Works seamlessly on Linux, Windows, and macOS.

# Tech Stack
- Language: C++17
- Build System: CMake
- Testing: Google Test (GTest)

# Performance Benchmarks
- Matches 100K orders in under 200 NCUs
- Matches 1M orders within the required ≤1500 NCUs threshold
