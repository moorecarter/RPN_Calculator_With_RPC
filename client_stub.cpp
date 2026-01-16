// 1. Create a socket
// 2. Increment the serial number
// 3. Marshal the metadata and parameters
// (including which function is being called)
// 4. Send message to server
// 5. Begin Loop
// 6.
// Receive a message (using recvfrom)
// 7.
// Unmarshall the potential result (and metadata)
// 8.
// Verify magic and version
// 9.
// Verify serial number (back to 5 if incorrect)
// 10.
// use the result
// 11.End Loop