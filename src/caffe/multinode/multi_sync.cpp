/*
All modification made by Intel Corporation: © 2016 Intel Corporation

All contributions by the University of California:
Copyright (c) 2014, 2015, The Regents of the University of California (Regents)
All rights reserved.

All other contributions:
Copyright (c) 2014, 2015, the respective contributors
All rights reserved.
For the list of contributors go to https://github.com/BVLC/caffe/blob/master/CONTRIBUTORS.md


Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifdef USE_MLSL

#include "caffe/multinode/multi_sync.hpp"

namespace caffe {

template<typename Dtype>
MultiSync<Dtype>::MultiSync(shared_ptr<Solver<Dtype> > root_solver)
        : solver(boost::make_shared<MultiSolver<Dtype> >(root_solver)),
          snapshot_per_iters(root_solver->param().snapshot()),
          layers(root_solver->net()->layers()),
          net(root_solver->net()),
          net_params(root_solver->net()->learnable_params()) {
  root_solver->param().set_disabled_update(true);
  if (!is_root) root_solver->param().clear_snapshot();
  if (!is_root) root_solver->param().set_snapshot_after_train(false);

  if (root_solver->iter() == 0)
    root_solver->set_iter(1);

  layer_param_ids.resize(layers.size());

  for (int layer_id = 0; layer_id < layers.size(); layer_id++) {
    shared_ptr<Layer<Dtype> > layer = layers[layer_id];

    /* cache param ids */
    layer_param_ids[layer_id] = net->get_layer_learnable_param_ids(layer_id);
  }
}

  INSTANTIATE_CLASS(MultiSync);
} // namespace caffe

#endif /* USE_MLSL */
