# Interface Design

## Blocks

### BaseBlock

| type | indentifier | default value    |     |
| ---- | ----------- | ---------------- | --- |
| int  | color       | No default value |     |

| return type | indentifier | parameters  | remarks           |
| ----------- | ----------- | ----------- | ----------------- |
| void        | crash       | (void)      | call ui to remove |
| -           | BaseBlock   | (int color) |                   |

### BoomBlock:BaseBlock

| return type | indentifier | parameters  | remarks               |
| ----------- | ----------- | ----------- | --------------------- |
| void        | crash       | (void)      | call ui to remove     |
| -           | BaseBlock   | (int color) | call BaseBlock(color) |

### LineClearBlock:BaseBlock

| return type | indentifier | parameters  | remarks               |
| ----------- | ----------- | ----------- | --------------------- |
| void        | crash       | (void)      | call ui to remove     |
| -           | BaseBlock   | (int color) | call BaseBlock(color) |

## BlocksUI

| typename               | indentifier | default value |     |
| ---------------------- | ----------- | ------------- | --- |
| const int              | col         |               |     |
| const int              | row         |               |     |
| valarray\<BaseBlocks\> | blocks      |               |     |

| return type | indentifier  | parameters                        | remarks                      |
| ----------- | ------------ | --------------------------------- | ---------------------------- |
| void        | addBlock     | (int col)                         | Create a block at column col |
| void        | replaceBlock | (int col,int row,BaseBlock Block) |                              |

## RankList

| return type             | indentifier | parameters |
| ----------------------- | ----------- | ---------- |
| const vector\<Record\>& | getRankList | (void)     |
| void                    | newRecord   | (Record)   |

You should learn how to use `sort`, `const_cast` and `vector`.

Every modifing operator should write to disk immediately.

### Record

| typename | indentifier | default value    |     |
| -------- | ----------- | ---------------- | --- |
| string   | username    | No default value |     |
| int      | score       | No default value |     |

## Core

| typename  | indentifier | default value       |     |
| --------- | ----------- | ------------------- | --- |
| BlocksUI& | game        | Reference to MainUI |     |

| return type | indentifier | parameters                                | remarks                 |
| ----------- | ----------- | ----------------------------------------- | ----------------------- |
| -           | Core        | (BlocksUI& game)                          | Create Core Class       |
| bool        | check       | (int col_a,int row_a,int col_b,int row_b) | check if exists a crash |
| bool        | do_crash    | (void)                                    | call ui to do crash     |

## Network

