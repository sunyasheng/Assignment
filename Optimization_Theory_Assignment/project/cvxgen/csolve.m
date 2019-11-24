% csolve  Solves a custom quadratic program very rapidly.
%
% [vars, status] = csolve(params, settings)
%
% solves the convex optimization problem
%
%   minimize(quad_form(x_0, Q) + quad_form(x_1, Q) + quad_form(x_2, Q) + quad_form(x_3, Q) + quad_form(x_4, Q) + quad_form(x_5, Q) + quad_form(x_6, Q) + quad_form(x_7, Q) + quad_form(x_8, Q) + quad_form(x_9, Q) + quad_form(x_10, Q) + quad_form(x_11, Q) + quad_form(x_12, Q) + quad_form(x_13, Q) + quad_form(x_14, Q) + quad_form(x_15, Q) + quad_form(x_16, Q) + quad_form(x_17, Q) + quad_form(x_18, Q) + quad_form(x_19, Q) + quad_form(x_20, Q) + quad_form(x_21, Q) + quad_form(x_22, Q) + quad_form(x_23, Q) + quad_form(x_24, Q) + quad_form(x_25, Q) + gamma*(quad_form(F_1 - F_0, eye(1)) + quad_form(F_2 - F_1, eye(1)) + quad_form(F_3 - F_2, eye(1)) + quad_form(F_4 - F_3, eye(1)) + quad_form(F_5 - F_4, eye(1)) + quad_form(F_6 - F_5, eye(1)) + quad_form(F_7 - F_6, eye(1)) + quad_form(F_8 - F_7, eye(1)) + quad_form(F_9 - F_8, eye(1)) + quad_form(F_10 - F_9, eye(1)) + quad_form(F_11 - F_10, eye(1)) + quad_form(F_12 - F_11, eye(1)) + quad_form(F_13 - F_12, eye(1)) + quad_form(F_14 - F_13, eye(1)) + quad_form(F_15 - F_14, eye(1)) + quad_form(F_16 - F_15, eye(1)) + quad_form(F_17 - F_16, eye(1)) + quad_form(F_18 - F_17, eye(1)) + quad_form(F_19 - F_18, eye(1)) + quad_form(F_20 - F_19, eye(1)) + quad_form(F_21 - F_20, eye(1)) + quad_form(F_22 - F_21, eye(1)) + quad_form(F_23 - F_22, eye(1)) + quad_form(F_24 - F_23, eye(1)) + quad_form(F_25 - F_24, eye(1))) + sig_env*S_env_15 + sig_env*S_env_16 + sig_env*S_env_17 + sig_env*S_env_18 + sig_env*S_env_19 + sig_env*S_env_20 + sig_env*S_env_21 + sig_env*S_env_22 + sig_env*S_env_23 + sig_env*S_env_24 + sig_env*S_env_25 + sig_env*S_env_26 + sig_tra*S_tra_15 + sig_tra*S_tra_16 + sig_tra*S_tra_17 + sig_tra*S_tra_18 + sig_tra*S_tra_19 + sig_tra*S_tra_20 + sig_tra*S_tra_21 + sig_tra*S_tra_22 + sig_tra*S_tra_23 + sig_tra*S_tra_24 + sig_tra*S_tra_25 + sig_tra*S_tra_26)
%   subject to
%     x_0 == x0
%     x_1 == A*x_0 + B*F_0 + d
%     x_2 == A*x_1 + B*F_1 + d
%     x_3 == A*x_2 + B*F_2 + d
%     x_4 == A*x_3 + B*F_3 + d
%     x_5 == A*x_4 + B*F_4 + d
%     x_6 == A*x_5 + B*F_5 + d
%     x_7 == A*x_6 + B*F_6 + d
%     x_8 == A*x_7 + B*F_7 + d
%     x_9 == A*x_8 + B*F_8 + d
%     x_10 == A*x_9 + B*F_9 + d
%     x_11 == A*x_10 + B*F_10 + d
%     x_12 == A*x_11 + B*F_11 + d
%     x_13 == A*x_12 + B*F_12 + d
%     x_14 == A*x_13 + B*F_13 + d
%     x_15 == A*x_14 + B*F_14 + d
%     x_16 == A*x_15 + B*F_15 + d
%     x_17 == A*x_16 + B*F_16 + d
%     x_18 == A*x_17 + B*F_17 + d
%     x_19 == A*x_18 + B*F_18 + d
%     x_20 == A*x_19 + B*F_19 + d
%     x_21 == A*x_20 + B*F_20 + d
%     x_22 == A*x_21 + B*F_21 + d
%     x_23 == A*x_22 + B*F_22 + d
%     x_24 == A*x_23 + B*F_23 + d
%     x_25 == A*x_24 + B*F_24 + d
%     x_26 == A*x_25 + B*F_25 + d
%     abs(F_0) <= F_max
%     abs(F_1) <= F_max
%     abs(F_2) <= F_max
%     abs(F_3) <= F_max
%     abs(F_4) <= F_max
%     abs(F_5) <= F_max
%     abs(F_6) <= F_max
%     abs(F_7) <= F_max
%     abs(F_8) <= F_max
%     abs(F_9) <= F_max
%     abs(F_10) <= F_max
%     abs(F_11) <= F_max
%     abs(F_12) <= F_max
%     abs(F_13) <= F_max
%     abs(F_14) <= F_max
%     abs(F_15) <= F_max
%     abs(F_16) <= F_max
%     abs(F_17) <= F_max
%     abs(F_18) <= F_max
%     abs(F_19) <= F_max
%     abs(F_20) <= F_max
%     abs(F_21) <= F_max
%     abs(F_22) <= F_max
%     abs(F_23) <= F_max
%     abs(F_24) <= F_max
%     abs(F_25) <= F_max
%     abs(F_1 - F_0) <= F_max_slew
%     abs(F_2 - F_1) <= F_max_slew
%     abs(F_3 - F_2) <= F_max_slew
%     abs(F_4 - F_3) <= F_max_slew
%     abs(F_5 - F_4) <= F_max_slew
%     abs(F_6 - F_5) <= F_max_slew
%     abs(F_7 - F_6) <= F_max_slew
%     abs(F_8 - F_7) <= F_max_slew
%     abs(F_9 - F_8) <= F_max_slew
%     abs(F_10 - F_9) <= F_max_slew
%     abs(F_11 - F_10) <= F_max_slew
%     abs(F_12 - F_11) <= F_max_slew
%     abs(F_13 - F_12) <= F_max_slew
%     abs(F_14 - F_13) <= F_max_slew
%     abs(F_15 - F_14) <= F_max_slew
%     abs(F_16 - F_15) <= F_max_slew
%     abs(F_17 - F_16) <= F_max_slew
%     abs(F_18 - F_17) <= F_max_slew
%     abs(F_19 - F_18) <= F_max_slew
%     abs(F_20 - F_19) <= F_max_slew
%     abs(F_21 - F_20) <= F_max_slew
%     abs(F_22 - F_21) <= F_max_slew
%     abs(F_23 - F_22) <= F_max_slew
%     abs(F_24 - F_23) <= F_max_slew
%     abs(F_25 - F_24) <= F_max_slew
%     H*x_15 <= G_15 + S_env_15 + S_tra_15
%     H*x_16 <= G_16 + S_env_16 + S_tra_16
%     H*x_17 <= G_17 + S_env_17 + S_tra_17
%     H*x_18 <= G_18 + S_env_18 + S_tra_18
%     H*x_19 <= G_19 + S_env_19 + S_tra_19
%     H*x_20 <= G_20 + S_env_20 + S_tra_20
%     H*x_21 <= G_21 + S_env_21 + S_tra_21
%     H*x_22 <= G_22 + S_env_22 + S_tra_22
%     H*x_23 <= G_23 + S_env_23 + S_tra_23
%     H*x_24 <= G_24 + S_env_24 + S_tra_24
%     H*x_25 <= G_25 + S_env_25 + S_tra_25
%     H*x_26 <= G_26 + S_env_26 + S_tra_26
%     S_env_15 >= Pos
%     S_env_16 >= Pos
%     S_env_17 >= Pos
%     S_env_18 >= Pos
%     S_env_19 >= Pos
%     S_env_20 >= Pos
%     S_env_21 >= Pos
%     S_env_22 >= Pos
%     S_env_23 >= Pos
%     S_env_24 >= Pos
%     S_env_25 >= Pos
%     S_env_26 >= Pos
%     S_tra_15 >= Pos
%     S_tra_16 >= Pos
%     S_tra_17 >= Pos
%     S_tra_18 >= Pos
%     S_tra_19 >= Pos
%     S_tra_20 >= Pos
%     S_tra_21 >= Pos
%     S_tra_22 >= Pos
%     S_tra_23 >= Pos
%     S_tra_24 >= Pos
%     S_tra_25 >= Pos
%     S_tra_26 >= Pos
%
% with variables
%      F_0   1 x 1
%      F_1   1 x 1
%      F_2   1 x 1
%      F_3   1 x 1
%      F_4   1 x 1
%      F_5   1 x 1
%      F_6   1 x 1
%      F_7   1 x 1
%      F_8   1 x 1
%      F_9   1 x 1
%     F_10   1 x 1
%     F_11   1 x 1
%     F_12   1 x 1
%     F_13   1 x 1
%     F_14   1 x 1
%     F_15   1 x 1
%     F_16   1 x 1
%     F_17   1 x 1
%     F_18   1 x 1
%     F_19   1 x 1
%     F_20   1 x 1
%     F_21   1 x 1
%     F_22   1 x 1
%     F_23   1 x 1
%     F_24   1 x 1
%     F_25   1 x 1
% S_env_15   2 x 1
% S_env_16   2 x 1
% S_env_17   2 x 1
% S_env_18   2 x 1
% S_env_19   2 x 1
% S_env_20   2 x 1
% S_env_21   2 x 1
% S_env_22   2 x 1
% S_env_23   2 x 1
% S_env_24   2 x 1
% S_env_25   2 x 1
% S_env_26   2 x 1
% S_tra_15   2 x 1
% S_tra_16   2 x 1
% S_tra_17   2 x 1
% S_tra_18   2 x 1
% S_tra_19   2 x 1
% S_tra_20   2 x 1
% S_tra_21   2 x 1
% S_tra_22   2 x 1
% S_tra_23   2 x 1
% S_tra_24   2 x 1
% S_tra_25   2 x 1
% S_tra_26   2 x 1
%      x_0   4 x 1
%      x_1   4 x 1
%      x_2   4 x 1
%      x_3   4 x 1
%      x_4   4 x 1
%      x_5   4 x 1
%      x_6   4 x 1
%      x_7   4 x 1
%      x_8   4 x 1
%      x_9   4 x 1
%     x_10   4 x 1
%     x_11   4 x 1
%     x_12   4 x 1
%     x_13   4 x 1
%     x_14   4 x 1
%     x_15   4 x 1
%     x_16   4 x 1
%     x_17   4 x 1
%     x_18   4 x 1
%     x_19   4 x 1
%     x_20   4 x 1
%     x_21   4 x 1
%     x_22   4 x 1
%     x_23   4 x 1
%     x_24   4 x 1
%     x_25   4 x 1
%     x_26   4 x 1
%
% and parameters
%        A   4 x 4
%        B   4 x 1
%    F_max   1 x 1
% F_max_slew   1 x 1
%     G_15   2 x 1
%     G_16   2 x 1
%     G_17   2 x 1
%     G_18   2 x 1
%     G_19   2 x 1
%     G_20   2 x 1
%     G_21   2 x 1
%     G_22   2 x 1
%     G_23   2 x 1
%     G_24   2 x 1
%     G_25   2 x 1
%     G_26   2 x 1
%        H   2 x 4
%      Pos   2 x 1    positive
%        Q   4 x 4    PSD
%        d   4 x 1
%    gamma   1 x 1    positive
%  sig_env   1 x 2    positive
%  sig_tra   1 x 2    positive
%       x0   4 x 1
%
% Note:
%   - Check status.converged, which will be 1 if optimization succeeded.
%   - You don't have to specify settings if you don't want to.
%   - To hide output, use settings.verbose = 0.
%   - To change iterations, use settings.max_iters = 20.
%   - You may wish to compare with cvxsolve to check the solver is correct.
%
% Specify params.A, ..., params.x0, then run
%   [vars, status] = csolve(params, settings)
% Produced by CVXGEN, 2018-06-02 01:51:06 -0400.
% CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com.
% The code in this file is Copyright (C) 2006-2017 Jacob Mattingley.
% CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial
% applications without prior written permission from Jacob Mattingley.

% Filename: csolve.m.
% Description: Help file for the Matlab solver interface.
