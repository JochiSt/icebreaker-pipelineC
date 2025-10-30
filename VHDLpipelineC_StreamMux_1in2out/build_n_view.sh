#!/bin/bash
make pipelinec_clean pipelinec && hdlmake && make sim_pre_cmd simulation sim_post_cmd view
