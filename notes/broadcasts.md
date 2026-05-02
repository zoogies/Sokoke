# broadcasts

Event    = an input fact that needs routing
Signal   = a notification fact that needs broadcasting
Command  = a requested mutation that needs execution

| Mechanism | Meaning                                         | Direction                      | Timing                       | Can be handled/consumed? |
| --------- | ----------------------------------------------- | ------------------------------ | ---------------------------- | ------------------------ |
| Event     | “This happened; who gets first shot at it?”     | external/input → engine/layers | usually immediate this frame | yes                      |
| Signal    | “This happened; anyone interested can observe.” | system → observers             | immediate or queued          | usually no               |
| Command   | “Do this.”                                      | caller → owner/executor        | often deferred               | no, it succeeds/fails    |
