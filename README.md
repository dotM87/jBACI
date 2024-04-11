# jBACI-1.4.5
jBACI Concurrency Simulator. Forked from http://www.weizmann.ac.il/sci-tea/benari/software-and-learning-materials/jbaci-concurrency-simulator and ported to Linux 

## Description
jBACI is a concurrency simulator that allows you to explore and understand fundamental concepts of concurrent programming and thread management in Pascal and C.

## Installation
To use jBACI, follow these steps:

1. Install Java Development Kit (JDK) on your system. You can download it from [Oracle's website](https://www.oracle.com/java/technologies/downloads/) or install it using your package manager:
    ```bash
    sudo apt-get install openjdk-11-jdk  # For Ubuntu/Debian
    ``` 
2. Clone the repository:
    ```bash
    git clone https://github.com/dotM87/jBACI-1.4.5.git
    ``` 

3. Change to the project directory:
    ```bash
    cd jBACI-1.4.5
    ```

4. Copy the binaries `bacc` and `bapas` from the `bin` directory to `usr/bin` as superuser:
    ```bash
    sudo cp -r bin/bacc /usr/bin/
    sudo cp -r bin/bapas /usr/bin/
    ```

5. Change the permissions of the binaries to make them executable:
    ```bash
    sudo chmod +x /usr/bin/bacc
    sudo chmod +x /usr/bin/bapas
    ```
6. Modify the configuration file `config.cfg` with the correct paths:
    ```bash
    sudo nano.cfg
    ```
    Update the paths in the configuration file according to your system setup.

## Usage
* Run the jBACI simulator:
    ```bash
    java -jar jbaci.jar
    ```
    Note: Run command in the project directory

## Documentation
* For detailed documentation, please refer to the `docs` directory in this repository.

## Examples
* Explore various examples in the `examples` directory to understand how to use jBACI effectively.


## License
This project is licensed under the GPL-2.0 License. See the [LICENSE](LICENSE) file for details.
