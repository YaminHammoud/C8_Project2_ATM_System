# ATM System (Console-based Application)

A secure and interactive ATM (Automated Teller Machine) simulation system developed in C++. The application provides a client-centric interface allowing authenticated users to perform standard banking transactions with real-time balance updates and persistent data storage.

### 🚀 Key Features:

* **Secure Authentication:** User login system that validates Account Numbers and PIN codes against stored credentials.
* **Quick Withdraw:** Fast cash withdrawal options with predefined amounts (e.g., $20, $50, $100, etc.).
* **Normal Withdraw:** Custom withdrawal module with integrated validation logic to ensure amounts are multiples of 5.
* **Instant Deposit:** Secure balance deposition with strict validation to prevent zero or negative inputs.
* **Balance Inquiry:** Real-time balance checking screen reflecting dynamic transaction changes.
* **Data Persistence:** Updates and transactions are saved dynamically back into a flat-file database (`Clients.txt`).

### 🛠 Technical Skills Demonstrated:

* **Data Structures:** Utilized custom structures (`struct`) and dynamic arrays (`std::vector`) to handle runtime client data.
* **File Handling (I/O):** Implemented stream tokenization and custom string splitting parsing techniques (`#//#` delimiter) to read/write database records.
* **Input Validation & Buffer Management:** Prevented infinite loops and menu crashes using advanced input buffer clearing.
* **State Management:** Handled system workflow cleanly using strongly-typed enumerations (`enum`) and switch-case control structures.
