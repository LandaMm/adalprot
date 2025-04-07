# AdalProt - Home Server Protocol (HSP)

AdalProt is a lightweight and secure communication library for local data transfer between services in a home server environment. It defines a custom, protocol-agnostic data transfer format designed for fast, reliable, and encrypted communication between modules running on a single host.

## Problem

In many home server setups, multiple services such as configuration management, file storage, and data processing need to communicate with each other. Existing protocols like HTTP are overkill for these local, trusted interactions. AdalProt solves this problem by providing a minimal, efficient, and secure way for services to exchange data.

## Features

- **Custom Data Format:** A simple and extensible format that allows easy communication between services. Each message is prefixed with a magic byte sequence and version for easy identification.
- **Security:** Built-in support for optional encryption and authentication of data to ensure secure transmission.
- **Lightweight:** No unnecessary overhead like HTTP headers, ideal for communication between local services on the same host.
- **Extensible:** Supports various data types in the payload (JSON, binary, etc.) and can be easily extended with additional features or modules.

## How It Works

AdalProt uses a simple, binary format for data transfer:

`<magic_bytes><version><headers_length><payload_length><headers><payload>`

- **Magic Bytes:** A unique identifier for the protocol.
- **Version:** Allows future protocol versions to coexist.
- **Headers:** Key-value pairs that provide metadata (e.g., content type, command).
- **Payload:** The actual data being transferred between services (e.g., JSON, binary data).

Each service communicates with others via this protocol, sending structured messages containing commands and data. Whether it's retrieving configuration settings, storing files, or processing data, AdalProt ensures secure and efficient communication within your home server.

## Integration

AdalProt is designed to be easily integrated into your custom services. It provides a shared library (`adalprot.so`) that you can link to your projects. It is built using CMake, ensuring compatibility across multiple platforms.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

