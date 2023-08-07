#!/bin/bash

bazel run generators/sbm_main -- --output_file="/home/ubuntu/outputs/SBM_5_homogeneous_0.1_0.03.ungraph.txt" --cmty_output_file="/home/ubuntu/outputs/SBM_5_homogeneous_0.1_0.03.cmty.txt" --p_in=0.1 --p_out=0.03 --n_block=5

bazel run generators/sbm_main -- --output_file="/home/ubuntu/outputs/SBM_5_homogeneous_0.3_0.1.ungraph.txt" --cmty_output_file="/home/ubuntu/outputs/SBM_5_homogeneous_0.3_0.1.cmty.txt" --p_in=0.3 --p_out=0.1 --n_block=5

bazel run generators/sbm_main -- --output_file="/home/ubuntu/outputs/SBM_50_homogeneous_0.3_0.1.ungraph.txt" --cmty_output_file="/home/ubuntu/outputs/SBM_50_homogeneous_0.3_0.1.cmty.txt" --p_in=0.3 --p_out=0.1 --n_block=50

bazel run generators/sbm_main -- --output_file="/home/ubuntu/outputs/SBM_50_homogeneous_0.6_0.1.ungraph.txt" --cmty_output_file="/home/ubuntu/outputs/SBM_50_homogeneous_0.6_0.1.cmty.txt" --p_in=0.6 --p_out=0.1 --n_block=50

bazel run generators/sbm_main -- --output_file="/home/ubuntu/outputs/SBM_5_inhomogeneous_0.3_0.1.ungraph.txt" --cmty_output_file="/home/ubuntu/outputs/SBM_5_inhomogeneous_0.3_0.1.cmty.txt" --p_in=0.3 --p_out=0.1 --n_block=5 --is_homo=false

bazel run generators/sbm_main -- --output_file="/home/ubuntu/outputs/SBM_50_inhomogeneous_0.6_0.1.ungraph.txt" --cmty_output_file="/home/ubuntu/outputs/SBM_50_inhomogeneous_0.6_0.1.cmty.txt" --p_in=0.6 --p_out=0.1 --n_block=50 --is_homo=false