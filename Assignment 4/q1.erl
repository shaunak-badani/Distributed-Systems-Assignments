-module(q1).
-export([main/1, spawnProcesses/4, send_and_receive_token/3, send_and_receive_token/2, spawnRemainingProcesses / 5, print_to_file/3]).

print_to_file(Outp_filename, StrVal,  Arr) ->
    file:write_file(Outp_filename, io_lib:fwrite(StrVal, Arr), [append]).

send_and_receive_token(0, Token, Outp_filename) ->
    receive
        {sender_id, PID} ->
            PID ! {token, Token, 0}
    end,
    receive
        {token, Token, Process_ID} ->
            print_to_file(Outp_filename, "Process 0 received token ~w from process ~w. ~n", [Token, Process_ID])
    end.
send_and_receive_token(Rank, Outp_filename) ->
    receive
        {sender_id, PID} ->
            receive
                {token, Token, Process_ID} ->
                    print_to_file(Outp_filename, "Process ~w received token ~w from process ~w. ~n", [Rank, Token, Process_ID]),
                    PID ! {token, Token, Rank}
            end
    end.

spawnRemainingProcesses(Rank, N, Prev_ID, Root, Outp_filename) when Rank < N ->
    New_ID = spawn(q1, send_and_receive_token, [Rank, Outp_filename]),
    Prev_ID ! {sender_id, New_ID},
    spawnRemainingProcesses(Rank + 1, N, New_ID, Root, Outp_filename);
spawnRemainingProcesses(_, _, Prev_ID, Root, _) ->
    Prev_ID ! {sender_id, Root}.


spawnProcesses(0, N, Token, Outp_filename) ->
    Root = spawn(q1, send_and_receive_token, [0, Token, Outp_filename]),
    spawnRemainingProcesses(1, N, Root, Root, Outp_filename).


main([ Inp_Filename | Outp_filename]) ->
    file:delete(Outp_filename),
    {ok, FileReadBuffer} = file:open(Inp_Filename, [read]),
    {ok, [NoOfProcesses, Token]} = io:fread(FileReadBuffer, "", "~d ~d"),
    _ = spawnProcesses(0, NoOfProcesses, Token, Outp_filename).