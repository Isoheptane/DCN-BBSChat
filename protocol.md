## Layer Protocol Implementation:

### Basic Structure

| Content | Type | Size |
| ------: | :--: | :--: |
| Command Length | uint8 | 1 Byte |
| Command | string | Vary |
| Body... |

Body part will hold all data used in operations

## Client to Server

#### Command `heartbeat`
This command has no body, just used for keeping a connection.

### Authentication
#### Command `login`
User requests for login in.

| Content | Type | Size |
| ------: | :--: | :--: |
| Username Length | uint8 | 1 Byte |
| Username | string | Vary |
| Password | SHA-256 | 32 Byte |

### Command `register`
User reuqests to register as a new user.

| Content | Type | Size |
| ------: | :--: | :--: |
| Username Length | uint8 | 1 Byte |
| Username | string | Vary |
| Password | SHA-256 | 32 Byte |

### Joining Group or DM
#### Command `lobby`
User requests to back to lobby.

This command has no body.

#### Command `overview`
User requests to show a overview of the logged in user.

This command has no body.

#### Command `create_group`
User requests to create a group.

| Content | Type | Size |
| ------: | :--: | :--: |
| Group Name Length | uint8 | 1 Byte |
| Group Name | string | Vary |

#### Command `join_group`
User requests to join a existing group.

| Content | Type | Size |
| ------: | :--: | :--: |
| Group Name Length | uint8 | 1 Byte |
| Group Name | string | Vary |

#### Command `remove_group`
User requests to remove a group. The group can only be removed if the group is empty.

| Content | Type | Size |
| ------: | :--: | :--: |
| Group Name Length | uint8 | 1 Byte |
| Group Name | string | Vary |

#### Command `join_dm`
User requests to join direct message to a user.

| Content | Type | Size |
| ------: | :--: | :--: |
| Username Length | uint8 | 1 Byte |
| Username | string | Vary |

#### Command `list`
User requests to list users.

If the user is in a group, list users in the group, else list users that are online.

This command has no body.

### Messages
#### Command `message`
Send a message to the client.

| Content | Type | Size |
| ------: | :--: | :--: |
| Typename Length | uint8 | 1 Byte |
| Typename | string | Vary |
| Message Content Length | uint16 | 2 Byte |
| Message Content | string | Vary |

Message type can be "text" or "file_hint".

### File Operations
#### Command `file_download`
Request to download file from server.

| Content | Type | Size |
| ------: | :--: | :--: |
| File Name Length | uint16 | 2 Byte |
| File Name | string | Vary |

#### Command `file_delete`
Delete a file uploaded to the server.

| Content | Type | Size |
| ------: | :--: | :--: |
| File Name Length | uint16 | 2 Byte |
| File Name | string | Vary |

#### Command `file_upload`
Upload content to the server. The content will be appended to the end of the file.

A client should call file_upload repeatedly until the file is fully uploaded.

| Content | Type | Size |
| ------: | :--: | :--: |
| File Name Length | uint16 | 2 Byte |
| File Name | string | Vary |
| Appended Block Length | uint16 | 2 Byte |
| File Name | uint8[] | Vary |

### Command `logout`
User requests to logout

This command has no body.

## Server to Client
### Info List
#### Command `overview`
Notify the user of current status of the server, like online counts, groups and user's direct messages.
| Content | Type | Size |
| ------: | :--: | :--: |
| Online Count | uint8 | 1 Byte |
| Group Count | uint8 | 1 Byte |
| Group 1 Name Length | uint8 | 1 Byte |
| Group 1 Name | string | Vary |
| Group 1 Online Count | uint8 | 1 Byte |
| ... |
| Direct Message Users Count | uint8 | 1 Byte |
| DM User 1 Name Length | uint8 | 1 Byte |
| DM User 1 Name | string | Vary |
| DM User DM count | string | 1 Byte |

### User List
When the user requested to list users, send username of users that are online.
#### Command `user_list`
| Content | Type | Size |
| ------: | :--: | :--: |
| User count | uint8 | 1 Byte |
| User 1 Name Length | uint8 | 1 Byte |
| User 1 Name | string | Vary |
| User 2 Name Length | uint8 | 1 Byte |
| User 2 Name | string | Vary |
| ... |
| User N Name Length | uint8 | 1 Byte |
| User N Name | string | Vary |

### Message
#### Command `message`
Forward a message to the client.

| Content | Type | Size |
| ------: | :--: | :--: |
| Username Length | uint8 | 1 Byte |
| Username | string | Vary |
| Typename Length | uint8 | 1 Byte |
| Typename | string | Vary |
| Message Content Length | uint16 | 2 Byte |
| Message Content | string | Vary |

Message type can be 
"text", "file_hint", "join", "leave", "server", "history_hint", "welcome"

For "text", "file_hint", "join" and "leave", username is necessary.

For "text", "file_hint", "server" and "welcome", content is necessary.

### File Operations
#### Command `file_send`
Send file content to the client. A server should send until file is fully sent.

| Content | Type | Size |
| ------: | :--: | :--: |
| File Name Length | uint16 | 2 Byte |
| File Name | string | Vary |
| Appended Block Length | uint16 | 2 Byte |
| File Name | uint8[] | Vary |