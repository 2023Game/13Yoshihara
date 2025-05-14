xof 0303txt 0032

template XSkinMeshHeader {
  <3cf169ce-ff7c-44ab-93c0-f78f62d172e2>
  WORD nMaxSkinWeightsPerVertex;
  WORD nMaxSkinWeightsPerFace;
  WORD nBones;
}

template SkinWeights {
  <6f0d123b-bad2-4167-a0d0-80224f25fabb>
  STRING transformNodeName;
  DWORD nWeights;
  array DWORD vertexIndices[nWeights];
  array float weights[nWeights];
  Matrix4x4 matrixOffset;
}

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000,-1.000000, 0.000000,
     0.000000, 1.000000,-0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame Armature {
    FrameTransformMatrix {
       0.100000, 0.000000, 0.000000, 0.000000,
       0.000000,-0.000000, 0.100000, 0.000000,
       0.000000,-0.100000,-0.000000, 0.000000,
       0.000000, 0.000000, 0.000000, 1.000000;;
    }
    Frame Armature_mixamorig_Hips {
      FrameTransformMatrix {
         0.978109, 0.047466, 0.202608, 0.000000,
        -0.112615, 0.939481, 0.323563, 0.000000,
        -0.174988,-0.339296, 0.924260, 0.000000,
         0.279435,88.770279, 0.406725, 1.000000;;
      }
      Frame Armature_mixamorig_LeftUpLeg {
        FrameTransformMatrix {
          -0.977698, 0.033984, 0.207199, 0.000000,
           0.155277,-0.547238, 0.822448, 0.000000,
           0.141337, 0.836287, 0.529762, 0.000000,
           7.707784,-5.105717, 0.126751, 1.000000;;
        }
        Frame Armature_mixamorig_LeftLeg {
          FrameTransformMatrix {
             0.996441,-0.077687, 0.032700, 0.000000,
             0.003263,-0.352113,-0.935952, 0.000000,
             0.084226, 0.932728,-0.350607, 0.000000,
             0.000000,45.952507,-0.000001, 1.000000;;
          }
          Frame Armature_mixamorig_LeftFoot {
            FrameTransformMatrix {
               0.934789, 0.336410, 0.114006, 0.000000,
              -0.176155, 0.160350, 0.971214, 0.000000,
               0.308446,-0.927963, 0.209154, 0.000000,
              -0.000001,44.565876,-0.000003, 1.000000;;
            }
            Frame Armature_mixamorig_LeftToeBase {
              FrameTransformMatrix {
                 0.912024, 0.016736, 0.409795, 0.000000,
                -0.239298, 0.833178, 0.498548, 0.000000,
                -0.333089,-0.552751, 0.763884, 0.000000,
                -0.000003,18.311054,-0.000003, 1.000000;;
              }
              Frame Armature_mixamorig_LeftToe_End {
                FrameTransformMatrix {
                   1.000000, 0.000000, 0.000000, 0.000000,
                  -0.000000, 1.000000,-0.000000, 0.000000,
                  -0.000000, 0.000000, 1.000000, 0.000000,
                   0.000007, 6.608835,-0.000002, 1.000000;;
                }
                Frame Armature_mixamorig_LeftToe_End_end {
                  FrameTransformMatrix {
                     1.000000,-0.000000,-0.000000, 0.000000,
                     0.000000, 1.000000,-0.000000, 0.000000,
                     0.000000, 0.000000, 1.000000, 0.000000,
                     0.000001, 6.608832, 0.000000, 1.000000;;
                  }
                } // End of Armature_mixamorig_LeftToe_End_end
              } // End of Armature_mixamorig_LeftToe_End
            } // End of Armature_mixamorig_LeftToeBase
          } // End of Armature_mixamorig_LeftFoot
        } // End of Armature_mixamorig_LeftLeg
      } // End of Armature_mixamorig_LeftUpLeg
      Frame Armature_mixamorig_RightUpLeg {
        FrameTransformMatrix {
          -0.988465, 0.149381,-0.023396, 0.000000,
          -0.148774,-0.933311, 0.326794, 0.000000,
           0.026976, 0.326518, 0.944806, 0.000000,
          -7.707784,-5.105715, 0.340394, 1.000000;;
        }
        Frame Armature_mixamorig_RightLeg {
          FrameTransformMatrix {
             0.987453, 0.075717, 0.138577, 0.000000,
             0.035772, 0.747483,-0.663317, 0.000000,
            -0.153808, 0.659952, 0.735396, 0.000000,
             0.000001,45.947220, 0.000000, 1.000000;;
          }
          Frame Armature_mixamorig_RightFoot {
            FrameTransformMatrix {
               0.997392, 0.035504,-0.062839, 0.000000,
               0.063498,-0.017734, 0.997824, 0.000000,
               0.034312,-0.999212,-0.019942, 0.000000,
               0.000002,44.572094, 0.000001, 1.000000;;
            }
            Frame Armature_mixamorig_RightToeBase {
              FrameTransformMatrix {
                 0.950690,-0.017378,-0.309657, 0.000000,
                 0.226983, 0.719367, 0.656497, 0.000000,
                 0.211349,-0.694412, 0.687839, 0.000000,
                -0.000000,18.116953,-0.000006, 1.000000;;
              }
              Frame Armature_mixamorig_RightToe_End {
                FrameTransformMatrix {
                   1.000000,-0.000000,-0.000008, 0.000000,
                   0.000000, 1.000000, 0.000000, 0.000000,
                   0.000008,-0.000000, 1.000001, 0.000000,
                  -0.000002, 6.707265, 0.000002, 1.000000;;
                }
                Frame Armature_mixamorig_RightToe_End_end {
                  FrameTransformMatrix {
                     1.000000, 0.000000, 0.000000, 0.000000,
                    -0.000000, 1.000000,-0.000000, 0.000000,
                    -0.000000, 0.000000, 1.000000, 0.000000,
                    -0.000001, 6.707255,-0.000000, 1.000000;;
                  }
                } // End of Armature_mixamorig_RightToe_End_end
              } // End of Armature_mixamorig_RightToe_End
            } // End of Armature_mixamorig_RightToeBase
          } // End of Armature_mixamorig_RightFoot
        } // End of Armature_mixamorig_RightLeg
      } // End of Armature_mixamorig_RightUpLeg
      Frame Armature_mixamorig_Spine {
        FrameTransformMatrix {
           0.990737,-0.056513,-0.123474, 0.000000,
           0.109917, 0.867674, 0.484831, 0.000000,
           0.079736,-0.493912, 0.865848, 0.000000,
           0.000000, 9.170838, 0.205811, 1.000000;;
        }
        Frame Armature_mixamorig_Spine1 {
          FrameTransformMatrix {
             0.999428,-0.010931,-0.032026, 0.000000,
             0.012089, 0.999272, 0.036176, 0.000000,
             0.031608,-0.036542, 0.998832, 0.000000,
             0.000000,10.701995,-0.000005, 1.000000;;
          }
          Frame Armature_mixamorig_Spine2 {
            FrameTransformMatrix {
               0.999388,-0.012518,-0.032671, 0.000000,
               0.013688, 0.999264, 0.035834, 0.000000,
               0.032198,-0.036259, 0.998824, 0.000000,
               0.000000,12.230858, 0.000003, 1.000000;;
            }
            Frame Armature_mixamorig_LeftShoulder {
              FrameTransformMatrix {
                -0.144339, 0.077606,-0.986480, 0.000000,
                 0.779985,-0.604550,-0.161685, 0.000000,
                -0.608924,-0.792778, 0.026729, 0.000000,
                 5.180532,11.652114, 0.054668, 1.000000;;
              }
              Frame Armature_mixamorig_LeftArm {
                FrameTransformMatrix {
                   0.953075, 0.302606, 0.008845, 0.000000,
                  -0.250017, 0.770298, 0.586628, 0.000000,
                   0.170704,-0.561312, 0.809808, 0.000000,
                   0.000009,10.993589,-0.000006, 1.000000;;
                }
                Frame Armature_mixamorig_LeftForeArm {
                  FrameTransformMatrix {
                     0.554547, 0.807137, 0.202506, 0.000000,
                    -0.795813, 0.585516,-0.154444, 0.000000,
                    -0.243228,-0.075510, 0.967026, 0.000000,
                     0.000005,21.306475,-0.000002, 1.000000;;
                  }
                  Frame Armature_mixamorig_LeftHand {
                    FrameTransformMatrix {
                       0.939654, 0.341070, 0.026887, 0.000000,
                      -0.342082, 0.935323, 0.090287, 0.000000,
                       0.005647,-0.094036, 0.995553, 0.000000,
                       0.000000,24.281303, 0.000004, 1.000000;;
                    }
                    Frame Armature_mixamorig_LeftHandIndex1 {
                      FrameTransformMatrix {
                         0.972656, 0.219271, 0.076555, 0.000000,
                        -0.217979, 0.748105, 0.626757, 0.000000,
                         0.080158,-0.626306, 0.775445, 0.000000,
                        -4.000837,10.644613, 0.188333, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandIndex2 {
                        FrameTransformMatrix {
                           0.996850, 0.079295, 0.000920, 0.000000,
                          -0.059412, 0.739095, 0.670977, 0.000000,
                           0.052525,-0.668918, 0.741478, 0.000000,
                          -0.000049, 3.442461,-0.000020, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandIndex3 {
                          FrameTransformMatrix {
                             0.999441,-0.033208,-0.003949, 0.000000,
                             0.033441, 0.993393, 0.109774, 0.000000,
                             0.000278,-0.109844, 0.993948, 0.000000,
                            -0.000159, 3.228664, 0.000035, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandIndex4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000,-0.000000, 0.000000,
                               0.000000, 1.000001,-0.000000, 0.000000,
                               0.000000, 0.000000, 1.000000, 0.000000,
                               0.000202, 2.712040, 0.000002, 1.000000;;
                            }
                            Frame Armature_mixamorig_LeftHandIndex4_end {
                              FrameTransformMatrix {
                                 1.000000,-0.000000,-0.000000, 0.000000,
                                -0.000000, 1.000000,-0.000000, 0.000000,
                                -0.000000, 0.000000, 1.000000, 0.000000,
                                 0.000012, 2.712036,-0.000003, 1.000000;;
                              }
                            } // End of Armature_mixamorig_LeftHandIndex4_end
                          } // End of Armature_mixamorig_LeftHandIndex4
                        } // End of Armature_mixamorig_LeftHandIndex3
                      } // End of Armature_mixamorig_LeftHandIndex2
                    } // End of Armature_mixamorig_LeftHandIndex1
                    Frame Armature_mixamorig_LeftHandMiddle1 {
                      FrameTransformMatrix {
                         0.988514, 0.150721, 0.011130, 0.000000,
                        -0.123376, 0.762244, 0.635422, 0.000000,
                         0.087288,-0.629497, 0.772084, 0.000000,
                        -1.145582,10.269846, 0.098126, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandMiddle2 {
                        FrameTransformMatrix {
                           0.995925, 0.090128, 0.003097, 0.000000,
                          -0.059401, 0.629776, 0.774502, 0.000000,
                           0.067854,-0.771530, 0.632563, 0.000000,
                          -0.001405, 4.567479,-0.000009, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandMiddle3 {
                          FrameTransformMatrix {
                             0.999641,-0.025036,-0.009506, 0.000000,
                             0.026759, 0.947882, 0.317495, 0.000000,
                             0.001062,-0.317636, 0.948213, 0.000000,
                             0.000994, 4.214488, 0.000009, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandMiddle4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000, 0.000000, 0.000000,
                               0.000000, 1.000000, 0.000001, 0.000000,
                              -0.000000,-0.000001, 1.000000, 0.000000,
                               0.000400, 3.872930,-0.000037, 1.000000;;
                            }
                            Frame Armature_mixamorig_LeftHandMiddle4_end {
                              FrameTransformMatrix {
                                 1.000000, 0.000000,-0.000000, 0.000000,
                                 0.000000, 1.000000, 0.000000, 0.000000,
                                -0.000000,-0.000000, 1.000000, 0.000000,
                                 0.000005, 3.872907,-0.000005, 1.000000;;
                              }
                            } // End of Armature_mixamorig_LeftHandMiddle4_end
                          } // End of Armature_mixamorig_LeftHandMiddle4
                        } // End of Armature_mixamorig_LeftHandMiddle3
                      } // End of Armature_mixamorig_LeftHandMiddle2
                    } // End of Armature_mixamorig_LeftHandMiddle1
                    Frame Armature_mixamorig_LeftHandPinky1 {
                      FrameTransformMatrix {
                         0.971128, 0.218889, 0.094863, 0.000000,
                        -0.206143, 0.569830, 0.795486, 0.000000,
                         0.120067,-0.792074, 0.598500, 0.000000,
                         3.634707,10.279562, 0.214912, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandPinky2 {
                        FrameTransformMatrix {
                           0.999647, 0.021372, 0.015763, 0.000000,
                          -0.021231, 0.286603, 0.957814, 0.000000,
                           0.015952,-0.957811, 0.286956, 0.000000,
                           0.001713, 3.272568,-0.000038, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandPinky3 {
                          FrameTransformMatrix {
                             0.999925, 0.011847, 0.002917, 0.000000,
                            -0.011750, 0.870698, 0.491677, 0.000000,
                             0.003285,-0.491675, 0.870772, 0.000000,
                            -0.001547, 2.574717,-0.000015, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandPinky4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000, 0.000000, 0.000000,
                               0.000000, 1.000000,-0.000001, 0.000000,
                              -0.000000, 0.000001, 1.000000, 0.000000,
                              -0.000162, 2.249540, 0.000027, 1.000000;;
                            }
                            Frame Armature_mixamorig_LeftHandPinky4_end {
                              FrameTransformMatrix {
                                 1.000000, 0.000000,-0.000000, 0.000000,
                                -0.000000, 1.000000,-0.000000, 0.000000,
                                 0.000000, 0.000000, 1.000000, 0.000000,
                                 0.000002, 2.249508, 0.000000, 1.000000;;
                              }
                            } // End of Armature_mixamorig_LeftHandPinky4_end
                          } // End of Armature_mixamorig_LeftHandPinky4
                        } // End of Armature_mixamorig_LeftHandPinky3
                      } // End of Armature_mixamorig_LeftHandPinky2
                    } // End of Armature_mixamorig_LeftHandPinky1
                    Frame Armature_mixamorig_LeftHandRing1 {
                      FrameTransformMatrix {
                         0.993595, 0.112434, 0.011285, 0.000000,
                        -0.060776, 0.447541, 0.892195, 0.000000,
                         0.095262,-0.887167, 0.451508, 0.000000,
                         1.511706,10.276342, 0.094540, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandRing2 {
                        FrameTransformMatrix {
                           0.999627, 0.021517, 0.016822, 0.000000,
                          -0.027137, 0.712823, 0.700818, 0.000000,
                           0.003088,-0.701013, 0.713141, 0.000000,
                          -0.000597, 3.979047,-0.000046, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandRing3 {
                          FrameTransformMatrix {
                             0.999810,-0.019451, 0.001213, 0.000000,
                             0.018399, 0.962570, 0.270409, 0.000000,
                            -0.006427,-0.270335, 0.962745, 0.000000,
                             0.000093, 3.715590, 0.000052, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandRing4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000, 0.000000, 0.000000,
                              -0.000000, 1.000000,-0.000001, 0.000000,
                              -0.000000, 0.000001, 1.000000, 0.000000,
                               0.000483, 3.315382, 0.000002, 1.000000;;
                            }
                            Frame Armature_mixamorig_LeftHandRing4_end {
                              FrameTransformMatrix {
                                 1.000000, 0.000000,-0.000000, 0.000000,
                                -0.000000, 1.000000, 0.000000, 0.000000,
                                 0.000000,-0.000000, 1.000000, 0.000000,
                                 0.000003, 3.315383, 0.000006, 1.000000;;
                              }
                            } // End of Armature_mixamorig_LeftHandRing4_end
                          } // End of Armature_mixamorig_LeftHandRing4
                        } // End of Armature_mixamorig_LeftHandRing3
                      } // End of Armature_mixamorig_LeftHandRing2
                    } // End of Armature_mixamorig_LeftHandRing1
                    Frame Armature_mixamorig_LeftHandThumb1 {
                      FrameTransformMatrix {
                         0.852996, 0.481807,-0.200646, 0.000000,
                        -0.436656, 0.869385, 0.231301, 0.000000,
                         0.285881,-0.109686, 0.951966, 0.000000,
                        -3.268712, 2.020260, 1.309106, 1.000000;;
                      }
                      Frame Armature_mixamorig_LeftHandThumb2 {
                        FrameTransformMatrix {
                           0.993645,-0.052072,-0.099787, 0.000000,
                           0.072933, 0.973121, 0.218442, 0.000000,
                           0.085730,-0.224331, 0.970735, 0.000000,
                          -0.446725, 3.216760,-0.000012, 1.000000;;
                        }
                        Frame Armature_mixamorig_LeftHandThumb3 {
                          FrameTransformMatrix {
                             0.640120, 0.121729,-0.758569, 0.000000,
                             0.141056, 0.951962, 0.271794, 0.000000,
                             0.755214,-0.280982, 0.592200, 0.000000,
                             0.289031, 4.061810,-0.000001, 1.000000;;
                          }
                          Frame Armature_mixamorig_LeftHandThumb4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000,-0.000000, 0.000000,
                               0.000000, 1.000001,-0.000001, 0.000000,
                               0.000000, 0.000001, 1.000001, 0.000000,
                               0.157702, 3.553954,-0.000020, 1.000000;;
                            }
                            Frame Armature_mixamorig_LeftHandThumb4_end {
                              FrameTransformMatrix {
                                 1.000000,-0.000000, 0.000000, 0.000000,
                                 0.000000, 1.000000,-0.000000, 0.000000,
                                -0.000000, 0.000000, 1.000000, 0.000000,
                                 0.000002, 3.557460,-0.000001, 1.000000;;
                              }
                            } // End of Armature_mixamorig_LeftHandThumb4_end
                          } // End of Armature_mixamorig_LeftHandThumb4
                        } // End of Armature_mixamorig_LeftHandThumb3
                      } // End of Armature_mixamorig_LeftHandThumb2
                    } // End of Armature_mixamorig_LeftHandThumb1
                  } // End of Armature_mixamorig_LeftHand
                } // End of Armature_mixamorig_LeftForeArm
              } // End of Armature_mixamorig_LeftArm
            } // End of Armature_mixamorig_LeftShoulder
            Frame Armature_mixamorig_Neck {
              FrameTransformMatrix {
                 0.999573,-0.018540,-0.022547, 0.000000,
                 0.013516, 0.978571,-0.205465, 0.000000,
                 0.025873, 0.205072, 0.978404, 0.000000,
                 0.000001,13.759685, 0.000012, 1.000000;;
              }
              Frame Armature_mixamorig_Head {
                FrameTransformMatrix {
                   0.993772,-0.061389, 0.093002, 0.000000,
                   0.096052, 0.895015,-0.435572, 0.000000,
                  -0.056498, 0.441792, 0.895337, 0.000000,
                  -0.000001, 7.527333,-0.214810, 1.000000;;
                }
                Frame Armature_mixamorig_HeadTop_End {
                  FrameTransformMatrix {
                     1.000000,-0.000000, 0.000000, 0.000000,
                    -0.000000, 1.000000,-0.000000, 0.000000,
                     0.000000, 0.000000, 1.000000, 0.000000,
                    -0.000000,26.017389,-0.742494, 1.000000;;
                  }
                  Frame Armature_mixamorig_HeadTop_End_end {
                    FrameTransformMatrix {
                       1.000000,-0.000000,-0.000000, 0.000000,
                      -0.000000, 1.000000,-0.000000, 0.000000,
                       0.000000,-0.000000, 1.000000, 0.000000,
                      -0.000000,26.027990,-0.000007, 1.000000;;
                    }
                  } // End of Armature_mixamorig_HeadTop_End_end
                } // End of Armature_mixamorig_HeadTop_End
              } // End of Armature_mixamorig_Head
            } // End of Armature_mixamorig_Neck
            Frame Armature_mixamorig_RightShoulder {
              FrameTransformMatrix {
                 0.217676, 0.000918, 0.976021, 0.000000,
                -0.929996,-0.303265, 0.207697, 0.000000,
                 0.296184,-0.952906,-0.065159, 0.000000,
                -5.180532,11.651780, 0.039952, 1.000000;;
              }
              Frame Armature_mixamorig_RightArm {
                FrameTransformMatrix {
                   0.862010,-0.240847,-0.446017, 0.000000,
                   0.452224, 0.762894, 0.462047, 0.000000,
                   0.228981,-0.599989, 0.766539, 0.000000,
                  -0.000010,10.993592,-0.000006, 1.000000;;
                }
                Frame Armature_mixamorig_RightForeArm {
                  FrameTransformMatrix {
                     0.109541,-0.978408,-0.175273, 0.000000,
                     0.988678, 0.125443,-0.082348, 0.000000,
                     0.102556,-0.164268, 0.981069, 0.000000,
                     0.000000,21.306290, 0.000007, 1.000000;;
                  }
                  Frame Armature_mixamorig_RightHand {
                    FrameTransformMatrix {
                       0.989625,-0.143355,-0.009535, 0.000000,
                       0.142783, 0.973959, 0.176113, 0.000000,
                      -0.015960,-0.175648, 0.984324, 0.000000,
                       0.000000,24.291656,-0.000003, 1.000000;;
                    }
                    Frame Armature_mixamorig_RightHandIndex1 {
                      FrameTransformMatrix {
                         0.997239,-0.073718,-0.008907, 0.000000,
                         0.043270, 0.479439, 0.876508, 0.000000,
                        -0.060344,-0.874474, 0.481306, 0.000000,
                         3.876497,10.479914, 0.248472, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandIndex2 {
                        FrameTransformMatrix {
                           0.996214,-0.086718,-0.006212, 0.000000,
                           0.040175, 0.395811, 0.917453, 0.000000,
                          -0.077101,-0.914229, 0.397796, 0.000000,
                           0.001574, 3.405114, 0.000020, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandIndex3 {
                          FrameTransformMatrix {
                             0.999609,-0.001779, 0.027911, 0.000000,
                            -0.023135, 0.508172, 0.860945, 0.000000,
                            -0.015715,-0.861254, 0.507932, 0.000000,
                            -0.000145, 3.328505,-0.000010, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandIndex4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000, 0.000000, 0.000000,
                              -0.000000, 1.000000,-0.000000, 0.000000,
                              -0.000000, 0.000000, 1.000000, 0.000000,
                              -0.001411, 2.857764,-0.000019, 1.000000;;
                            }
                            Frame Armature_mixamorig_RightHandIndex4_end {
                              FrameTransformMatrix {
                                 1.000000, 0.000000,-0.000000, 0.000000,
                                -0.000000, 1.000000,-0.000000, 0.000000,
                                 0.000000, 0.000000, 1.000000, 0.000000,
                                 0.000001, 2.857756,-0.000001, 1.000000;;
                              }
                            } // End of Armature_mixamorig_RightHandIndex4_end
                          } // End of Armature_mixamorig_RightHandIndex4
                        } // End of Armature_mixamorig_RightHandIndex3
                      } // End of Armature_mixamorig_RightHandIndex2
                    } // End of Armature_mixamorig_RightHandIndex1
                    Frame Armature_mixamorig_RightHandMiddle1 {
                      FrameTransformMatrix {
                         0.991314,-0.128393,-0.028504, 0.000000,
                         0.074288, 0.367786, 0.926938, 0.000000,
                        -0.108529,-0.921004, 0.374129, 0.000000,
                         1.265793,10.730997,-0.044063, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandMiddle2 {
                        FrameTransformMatrix {
                           0.994842,-0.100397,-0.014510, 0.000000,
                           0.024148, 0.095462, 0.995140, 0.000000,
                          -0.098524,-0.990357, 0.097394, 0.000000,
                           0.001820, 4.302097, 0.000003, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandMiddle3 {
                          FrameTransformMatrix {
                             0.999805, 0.014802, 0.013100, 0.000000,
                            -0.019534, 0.841149, 0.540449, 0.000000,
                            -0.003020,-0.540600, 0.841274, 0.000000,
                             0.000390, 4.040844, 0.000009, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandMiddle4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000,-0.000000, 0.000000,
                              -0.000000, 1.000000, 0.000000, 0.000000,
                               0.000000,-0.000000, 1.000000, 0.000000,
                              -0.002185, 3.587913, 0.000009, 1.000000;;
                            }
                            Frame Armature_mixamorig_RightHandMiddle4_end {
                              FrameTransformMatrix {
                                 1.000000, 0.000000,-0.000000, 0.000000,
                                -0.000000, 1.000000, 0.000000, 0.000000,
                                -0.000000,-0.000000, 1.000000, 0.000000,
                                -0.000006, 3.587888, 0.000000, 1.000000;;
                              }
                            } // End of Armature_mixamorig_RightHandMiddle4_end
                          } // End of Armature_mixamorig_RightHandMiddle4
                        } // End of Armature_mixamorig_RightHandMiddle3
                      } // End of Armature_mixamorig_RightHandMiddle2
                    } // End of Armature_mixamorig_RightHandMiddle1
                    Frame Armature_mixamorig_RightHandPinky1 {
                      FrameTransformMatrix {
                         0.984668,-0.154751,-0.080500, 0.000000,
                         0.089232, 0.050319, 0.994739, 0.000000,
                        -0.149887,-0.986671, 0.063357, 0.000000,
                        -3.578569,10.364506, 0.239122, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandPinky2 {
                        FrameTransformMatrix {
                           0.999470,-0.029095,-0.014677, 0.000000,
                           0.028875, 0.581951, 0.812712, 0.000000,
                          -0.015104,-0.812704, 0.582482, 0.000000,
                          -0.001968, 3.211729, 0.000008, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandPinky3 {
                          FrameTransformMatrix {
                             0.999831,-0.017933,-0.003911, 0.000000,
                             0.017847, 0.900062, 0.435396, 0.000000,
                            -0.004287,-0.435393, 0.900230, 0.000000,
                             0.003090, 2.578152, 0.000022, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandPinky4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000,-0.000000, 0.000000,
                              -0.000000, 1.000000,-0.000001, 0.000000,
                               0.000000, 0.000001, 1.000000, 0.000000,
                              -0.001134, 2.342509,-0.000010, 1.000000;;
                            }
                            Frame Armature_mixamorig_RightHandPinky4_end {
                              FrameTransformMatrix {
                                 1.000000,-0.000000, 0.000000, 0.000000,
                                 0.000000, 1.000000, 0.000000, 0.000000,
                                -0.000000,-0.000000, 1.000000, 0.000000,
                                -0.000003, 2.342495, 0.000001, 1.000000;;
                              }
                            } // End of Armature_mixamorig_RightHandPinky4_end
                          } // End of Armature_mixamorig_RightHandPinky4
                        } // End of Armature_mixamorig_RightHandPinky3
                      } // End of Armature_mixamorig_RightHandPinky2
                    } // End of Armature_mixamorig_RightHandPinky1
                    Frame Armature_mixamorig_RightHandRing1 {
                      FrameTransformMatrix {
                         0.998312,-0.057762, 0.005966, 0.000000,
                         0.005221, 0.191611, 0.981456, 0.000000,
                        -0.057834,-0.979770, 0.191589, 0.000000,
                        -1.563632,10.580319, 0.239539, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandRing2 {
                        FrameTransformMatrix {
                           0.998796,-0.040340,-0.027908, 0.000000,
                           0.038371, 0.288068, 0.956841, 0.000000,
                          -0.030559,-0.956760, 0.289269, 0.000000,
                          -0.000531, 3.718163, 0.000001, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandRing3 {
                          FrameTransformMatrix {
                             0.999954, 0.007686,-0.005770, 0.000000,
                            -0.002316, 0.775385, 0.631485, 0.000000,
                             0.009327,-0.631442, 0.775366, 0.000000,
                            -0.000302, 3.643932, 0.000023, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandRing4 {
                            FrameTransformMatrix {
                               1.000000,-0.000000,-0.000000, 0.000000,
                               0.000000, 1.000001, 0.000001, 0.000000,
                              -0.000000,-0.000001, 1.000000, 0.000000,
                               0.000871, 3.025709,-0.000004, 1.000000;;
                            }
                            Frame Armature_mixamorig_RightHandRing4_end {
                              FrameTransformMatrix {
                                 1.000000, 0.000000, 0.000000, 0.000000,
                                 0.000000, 1.000000,-0.000000, 0.000000,
                                -0.000000,-0.000000, 1.000000, 0.000000,
                                -0.000001, 3.025683, 0.000001, 1.000000;;
                              }
                            } // End of Armature_mixamorig_RightHandRing4_end
                          } // End of Armature_mixamorig_RightHandRing4
                        } // End of Armature_mixamorig_RightHandRing3
                      } // End of Armature_mixamorig_RightHandRing2
                    } // End of Armature_mixamorig_RightHandRing1
                    Frame Armature_mixamorig_RightHandThumb1 {
                      FrameTransformMatrix {
                         0.925314,-0.181989, 0.332675, 0.000000,
                        -0.155531, 0.617973, 0.770661, 0.000000,
                        -0.345836,-0.764846, 0.543515, 0.000000,
                         3.351190, 2.208743, 1.381203, 1.000000;;
                      }
                      Frame Armature_mixamorig_RightHandThumb2 {
                        FrameTransformMatrix {
                           0.941423,-0.125402, 0.313046, 0.000000,
                           0.191107, 0.963230,-0.188857, 0.000000,
                          -0.277852, 0.237620, 0.930772, 0.000000,
                           0.503287, 3.358197, 0.000005, 1.000000;;
                        }
                        Frame Armature_mixamorig_RightHandThumb3 {
                          FrameTransformMatrix {
                             0.953529, 0.107793,-0.281358, 0.000000,
                            -0.177128, 0.955953,-0.234050, 0.000000,
                             0.243736, 0.273009, 0.930622, 0.000000,
                            -0.013890, 3.929743,-0.000001, 1.000000;;
                          }
                          Frame Armature_mixamorig_RightHandThumb4 {
                            FrameTransformMatrix {
                               1.000000, 0.000000,-0.000000, 0.000000,
                              -0.000000, 1.000000,-0.000000, 0.000000,
                              -0.000000, 0.000000, 1.000000, 0.000000,
                              -0.489374, 3.302327, 0.000033, 1.000000;;
                            }
                            Frame Armature_mixamorig_RightHandThumb4_end {
                              FrameTransformMatrix {
                                 1.000000, 0.000001,-0.000001, 0.000000,
                                -0.000001, 1.000000,-0.000000, 0.000000,
                                 0.000001, 0.000000, 1.000000, 0.000000,
                                -0.000002, 3.338405, 0.000009, 1.000000;;
                              }
                            } // End of Armature_mixamorig_RightHandThumb4_end
                          } // End of Armature_mixamorig_RightHandThumb4
                        } // End of Armature_mixamorig_RightHandThumb3
                      } // End of Armature_mixamorig_RightHandThumb2
                    } // End of Armature_mixamorig_RightHandThumb1
                  } // End of Armature_mixamorig_RightHand
                } // End of Armature_mixamorig_RightForeArm
              } // End of Armature_mixamorig_RightArm
            } // End of Armature_mixamorig_RightShoulder
          } // End of Armature_mixamorig_Spine2
        } // End of Armature_mixamorig_Spine1
      } // End of Armature_mixamorig_Spine
    } // End of Armature_mixamorig_Hips
    Frame Adventurer_Body {
      FrameTransformMatrix {
        99.999977, 0.000000,-0.000000, 0.000000,
         0.000000,-0.000028,-99.999977, 0.000000,
         0.000000,99.999969,-0.000028, 0.000000,
         0.000007, 0.000007,-0.000004, 1.000000;;
      }
    } // End of Adventurer_Body
    Frame Adventurer_Feet {
      FrameTransformMatrix {
        99.999977, 0.000001, 0.000000, 0.000000,
        -0.000002,-0.000028,-99.999977, 0.000000,
         0.000002,99.999969,-0.000028, 0.000000,
         0.000007, 0.000007,-0.000002, 1.000000;;
      }
    } // End of Adventurer_Feet
    Frame Adventurer_Head {
      FrameTransformMatrix {
        99.999977, 0.000000,-0.000000, 0.000000,
        -0.000000,-0.000028,-99.999977, 0.000000,
         0.000000,99.999969,-0.000028, 0.000000,
         0.000009,-0.000006,-0.000004, 1.000000;;
      }
    } // End of Adventurer_Head
    Frame Adventurer_Legs {
      FrameTransformMatrix {
        99.999977, 0.000001, 0.000000, 0.000000,
        -0.000002,-0.000028,-99.999977, 0.000000,
         0.000002,99.999969,-0.000028, 0.000000,
         0.000007, 0.000007,-0.000002, 1.000000;;
      }
    } // End of Adventurer_Legs
    Frame Backpack {
      FrameTransformMatrix {
        -26.076880, 0.000000,-0.000010, 0.000000,
        -0.000010, 0.000008,26.076878, 0.000000,
         0.000000,26.076880,-0.000008, 0.000000,
         0.000007,137.266464,-11.664410, 1.000000;;
      }
    } // End of Backpack
  } // End of Armature
} // End of Root
AnimationSet Global {
  Animation {
    {Adventurer_Body}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      1;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      2;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      3;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      4;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      5;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      6;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      7;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      8;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      9;4;-0.707107,-0.707107,-0.000000, 0.000000;;,
      10;4;-0.707107,-0.707107,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3;99.999977,99.999977,99.999969;;,
      1;3;99.999977,99.999977,99.999969;;,
      2;3;99.999977,99.999977,99.999969;;,
      3;3;99.999977,99.999977,99.999969;;,
      4;3;99.999977,99.999977,99.999969;;,
      5;3;99.999977,99.999977,99.999969;;,
      6;3;99.999977,99.999977,99.999969;;,
      7;3;99.999977,99.999977,99.999969;;,
      8;3;99.999977,99.999977,99.999969;;,
      9;3;99.999977,99.999977,99.999969;;,
      10;3;99.999977,99.999977,99.999969;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000007, 0.000007,-0.000004;;,
      1;3; 0.000007, 0.000007,-0.000004;;,
      2;3; 0.000007, 0.000007,-0.000004;;,
      3;3; 0.000007, 0.000007,-0.000004;;,
      4;3; 0.000007, 0.000007,-0.000004;;,
      5;3; 0.000007, 0.000007,-0.000004;;,
      6;3; 0.000007, 0.000007,-0.000004;;,
      7;3; 0.000007, 0.000007,-0.000004;;,
      8;3; 0.000007, 0.000007,-0.000004;;,
      9;3; 0.000007, 0.000007,-0.000004;;,
      10;3; 0.000007, 0.000007,-0.000004;;;
    }
  }
  Animation {
    {Adventurer_Feet}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      1;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      2;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      3;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      4;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      5;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      6;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      7;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      8;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      9;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      10;4;-0.707107,-0.707107, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3;99.999977,99.999977,99.999969;;,
      1;3;99.999977,99.999977,99.999969;;,
      2;3;99.999977,99.999977,99.999969;;,
      3;3;99.999977,99.999977,99.999969;;,
      4;3;99.999977,99.999977,99.999969;;,
      5;3;99.999977,99.999977,99.999969;;,
      6;3;99.999977,99.999977,99.999969;;,
      7;3;99.999977,99.999977,99.999969;;,
      8;3;99.999977,99.999977,99.999969;;,
      9;3;99.999977,99.999977,99.999969;;,
      10;3;99.999977,99.999977,99.999969;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000007, 0.000007,-0.000002;;,
      1;3; 0.000007, 0.000007,-0.000002;;,
      2;3; 0.000007, 0.000007,-0.000002;;,
      3;3; 0.000007, 0.000007,-0.000002;;,
      4;3; 0.000007, 0.000007,-0.000002;;,
      5;3; 0.000007, 0.000007,-0.000002;;,
      6;3; 0.000007, 0.000007,-0.000002;;,
      7;3; 0.000007, 0.000007,-0.000002;;,
      8;3; 0.000007, 0.000007,-0.000002;;,
      9;3; 0.000007, 0.000007,-0.000002;;,
      10;3; 0.000007, 0.000007,-0.000002;;;
    }
  }
  Animation {
    {Adventurer_Head}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      1;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      2;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      3;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      4;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      5;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      6;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      7;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      8;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      9;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      10;4;-0.707107,-0.707107, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3;99.999977,99.999977,99.999969;;,
      1;3;99.999977,99.999977,99.999969;;,
      2;3;99.999977,99.999977,99.999969;;,
      3;3;99.999977,99.999977,99.999969;;,
      4;3;99.999977,99.999977,99.999969;;,
      5;3;99.999977,99.999977,99.999969;;,
      6;3;99.999977,99.999977,99.999969;;,
      7;3;99.999977,99.999977,99.999969;;,
      8;3;99.999977,99.999977,99.999969;;,
      9;3;99.999977,99.999977,99.999969;;,
      10;3;99.999977,99.999977,99.999969;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000009,-0.000006,-0.000004;;,
      1;3; 0.000009,-0.000006,-0.000004;;,
      2;3; 0.000009,-0.000006,-0.000004;;,
      3;3; 0.000009,-0.000006,-0.000004;;,
      4;3; 0.000009,-0.000006,-0.000004;;,
      5;3; 0.000009,-0.000006,-0.000004;;,
      6;3; 0.000009,-0.000006,-0.000004;;,
      7;3; 0.000009,-0.000006,-0.000004;;,
      8;3; 0.000009,-0.000006,-0.000004;;,
      9;3; 0.000009,-0.000006,-0.000004;;,
      10;3; 0.000009,-0.000006,-0.000004;;;
    }
  }
  Animation {
    {Adventurer_Legs}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      1;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      2;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      3;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      4;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      5;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      6;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      7;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      8;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      9;4;-0.707107,-0.707107, 0.000000,-0.000000;;,
      10;4;-0.707107,-0.707107, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3;99.999977,99.999977,99.999969;;,
      1;3;99.999977,99.999977,99.999969;;,
      2;3;99.999977,99.999977,99.999969;;,
      3;3;99.999977,99.999977,99.999969;;,
      4;3;99.999977,99.999977,99.999969;;,
      5;3;99.999977,99.999977,99.999969;;,
      6;3;99.999977,99.999977,99.999969;;,
      7;3;99.999977,99.999977,99.999969;;,
      8;3;99.999977,99.999977,99.999969;;,
      9;3;99.999977,99.999977,99.999969;;,
      10;3;99.999977,99.999977,99.999969;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000007, 0.000007,-0.000002;;,
      1;3; 0.000007, 0.000007,-0.000002;;,
      2;3; 0.000007, 0.000007,-0.000002;;,
      3;3; 0.000007, 0.000007,-0.000002;;,
      4;3; 0.000007, 0.000007,-0.000002;;,
      5;3; 0.000007, 0.000007,-0.000002;;,
      6;3; 0.000007, 0.000007,-0.000002;;,
      7;3; 0.000007, 0.000007,-0.000002;;,
      8;3; 0.000007, 0.000007,-0.000002;;,
      9;3; 0.000007, 0.000007,-0.000002;;,
      10;3; 0.000007, 0.000007,-0.000002;;;
    }
  }
  Animation {
    {Armature}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      1;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      2;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      3;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      4;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      5;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      6;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      7;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      8;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      9;4;-0.707107, 0.707107, 0.000000, 0.000000;;,
      10;4;-0.707107, 0.707107, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 0.100000, 0.100000, 0.100000;;,
      1;3; 0.100000, 0.100000, 0.100000;;,
      2;3; 0.100000, 0.100000, 0.100000;;,
      3;3; 0.100000, 0.100000, 0.100000;;,
      4;3; 0.100000, 0.100000, 0.100000;;,
      5;3; 0.100000, 0.100000, 0.100000;;,
      6;3; 0.100000, 0.100000, 0.100000;;,
      7;3; 0.100000, 0.100000, 0.100000;;,
      8;3; 0.100000, 0.100000, 0.100000;;,
      9;3; 0.100000, 0.100000, 0.100000;;,
      10;3; 0.100000, 0.100000, 0.100000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000, 0.000000, 0.000000;;,
      1;3; 0.000000, 0.000000, 0.000000;;,
      2;3; 0.000000, 0.000000, 0.000000;;,
      3;3; 0.000000, 0.000000, 0.000000;;,
      4;3; 0.000000, 0.000000, 0.000000;;,
      5;3; 0.000000, 0.000000, 0.000000;;,
      6;3; 0.000000, 0.000000, 0.000000;;,
      7;3; 0.000000, 0.000000, 0.000000;;,
      8;3; 0.000000, 0.000000, 0.000000;;,
      9;3; 0.000000, 0.000000, 0.000000;;,
      10;3; 0.000000, 0.000000, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_Hips}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.989913, 0.040427,-0.122605, 0.058365;;,
      1;4;-0.989216, 0.039260,-0.127881, 0.059633;;,
      2;4;-0.988196, 0.040077,-0.136952, 0.055738;;,
      3;4;-0.987530, 0.050729,-0.139004, 0.053752;;,
      4;4;-0.987657, 0.065792,-0.131834, 0.053157;;,
      5;4;-0.987791, 0.083149,-0.120943, 0.052223;;,
      6;4;-0.987127, 0.102763,-0.111082, 0.051773;;,
      7;4;-0.984897, 0.128317,-0.104513, 0.050880;;,
      8;4;-0.982155, 0.151738,-0.099924, 0.048604;;,
      9;4;-0.980857, 0.162392,-0.097643, 0.044883;;,
      10;4;-0.980032, 0.169091,-0.096323, 0.040836;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-1.012862,88.770279,-8.563274;;,
      1;3;-1.204789,88.770279,-7.900818;;,
      2;3;-1.561930,88.770279,-7.122106;;,
      3;3;-1.707635,88.770279,-6.144079;;,
      4;3;-1.445906,88.770279,-3.979228;;,
      5;3;-0.975095,88.770279,-1.385538;;,
      6;3;-0.544698,88.770279, 0.822010;;,
      7;3;-0.309453,88.770279, 2.489589;;,
      8;3;-0.116910,88.770279, 3.257987;;,
      9;3; 0.088355,88.770279, 2.172347;;,
      10;3; 0.279435,88.770279, 0.406725;;;
    }
  }
  Animation {
    {Armature_mixamorig_Spine}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.972415, 0.221659,-0.028884,-0.066650;;,
      1;4;-0.972537, 0.219497,-0.024338,-0.073481;;,
      2;4;-0.972356, 0.219497,-0.016648,-0.077891;;,
      3;4;-0.971963, 0.221257,-0.007235,-0.079256;;,
      4;4;-0.971182, 0.225936, 0.003821,-0.075792;;,
      5;4;-0.970262, 0.231382, 0.015522,-0.069380;;,
      6;4;-0.969092, 0.237105, 0.026324,-0.062848;;,
      7;4;-0.967747, 0.242935, 0.037348,-0.055254;;,
      8;4;-0.966453, 0.247930, 0.046627,-0.048215;;,
      9;4;-0.965606, 0.251054, 0.050571,-0.044946;;,
      10;4;-0.964917, 0.253582, 0.052650,-0.043120;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000, 9.170852, 0.205811;;,
      1;3; 0.000001, 9.170848, 0.205811;;,
      2;3; 0.000000, 9.170841, 0.205810;;,
      3;3; 0.000001, 9.170841, 0.205810;;,
      4;3; 0.000000, 9.170837, 0.205810;;,
      5;3; 0.000000, 9.170844, 0.205813;;,
      6;3;-0.000000, 9.170840, 0.205809;;,
      7;3; 0.000000, 9.170849, 0.205811;;,
      8;3;-0.000001, 9.170845, 0.205810;;,
      9;3; 0.000001, 9.170842, 0.205813;;,
      10;3; 0.000000, 9.170838, 0.205811;;;
    }
  }
  Animation {
    {Armature_mixamorig_Spine1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.999620, 0.019579,-0.007394,-0.017918;;,
      1;4;-0.999611, 0.018419,-0.006552,-0.019917;;,
      2;4;-0.999603, 0.018905,-0.005817,-0.020056;;,
      3;4;-0.999607, 0.019927,-0.004736,-0.019148;;,
      4;4;-0.999635, 0.020903,-0.002516,-0.016959;;,
      5;4;-0.999671, 0.021343, 0.000376,-0.014239;;,
      6;4;-0.999701, 0.021338, 0.003682,-0.011338;;,
      7;4;-0.999720, 0.020222, 0.008533,-0.008843;;,
      8;4;-0.999712, 0.018994, 0.012934,-0.006936;;,
      9;4;-0.999700, 0.018490, 0.014817,-0.006146;;,
      10;4;-0.999691, 0.018185, 0.015913,-0.005757;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000001,10.701986, 0.000000;;,
      1;3;-0.000000,10.701987,-0.000008;;,
      2;3;-0.000001,10.701994,-0.000006;;,
      3;3; 0.000001,10.701986, 0.000003;;,
      4;3; 0.000000,10.701997, 0.000006;;,
      5;3;-0.000000,10.701988, 0.000003;;,
      6;3;-0.000000,10.701980, 0.000002;;,
      7;3; 0.000000,10.701983, 0.000002;;,
      8;3;-0.000000,10.701981, 0.000001;;,
      9;3;-0.000000,10.701995, 0.000001;;,
      10;3; 0.000000,10.701995,-0.000005;;;
    }
  }
  Animation {
    {Armature_mixamorig_Spine2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.999493, 0.023539,-0.005105,-0.020842;;,
      1;4;-0.999475, 0.022444,-0.004255,-0.022967;;,
      2;4;-0.999433, 0.022953,-0.003575,-0.024394;;,
      3;4;-0.999395, 0.024078,-0.002686,-0.024972;;,
      4;4;-0.999395, 0.025446,-0.001008,-0.023699;;,
      5;4;-0.999415, 0.026668, 0.001116,-0.021395;;,
      6;4;-0.999447, 0.027446, 0.003600,-0.018412;;,
      7;4;-0.999523, 0.025942, 0.006560,-0.015424;;,
      8;4;-0.999598, 0.023550, 0.009717,-0.012449;;,
      9;4;-0.999653, 0.020896, 0.012940,-0.009492;;,
      10;4;-0.999684, 0.018029, 0.016222,-0.006553;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000001,12.230854, 0.000001;;,
      1;3; 0.000000,12.230865, 0.000005;;,
      2;3;-0.000001,12.230863, 0.000000;;,
      3;3;-0.000000,12.230855, 0.000001;;,
      4;3; 0.000000,12.230866, 0.000007;;,
      5;3;-0.000000,12.230862,-0.000005;;,
      6;3;-0.000000,12.230866, 0.000004;;,
      7;3; 0.000001,12.230854, 0.000019;;,
      8;3; 0.000000,12.230864, 0.000009;;,
      9;3;-0.000000,12.230859,-0.000009;;,
      10;3; 0.000000,12.230858, 0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_Neck}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.997667,-0.037538, 0.041734,-0.038859;;,
      1;4;-0.997325,-0.046557, 0.037887,-0.041708;;,
      2;4;-0.996993,-0.054042, 0.033601,-0.044221;;,
      3;4;-0.996879,-0.057090, 0.028884,-0.046239;;,
      4;4;-0.997046,-0.055750, 0.023391,-0.047380;;,
      5;4;-0.997305,-0.054051, 0.017062,-0.046582;;,
      6;4;-0.997399,-0.057131, 0.010846,-0.042587;;,
      7;4;-0.997127,-0.066749, 0.006603,-0.035207;;,
      8;4;-0.996430,-0.080166, 0.005762,-0.025827;;,
      9;4;-0.995466,-0.093370, 0.008043,-0.016301;;,
      10;4;-0.994554,-0.103196, 0.012171,-0.008058;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 0.999999, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 0.999999, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000001,13.759684, 0.000000;;,
      1;3; 0.000001,13.759672, 0.000003;;,
      2;3; 0.000002,13.759667,-0.000005;;,
      3;3;-0.000001,13.759681, 0.000012;;,
      4;3;-0.000000,13.759680, 0.000007;;,
      5;3; 0.000001,13.759671,-0.000002;;,
      6;3; 0.000001,13.759676, 0.000012;;,
      7;3; 0.000000,13.759694, 0.000004;;,
      8;3; 0.000001,13.759686,-0.000000;;,
      9;3; 0.000001,13.759685, 0.000008;;,
      10;3; 0.000001,13.759685, 0.000012;;;
    }
  }
  Animation {
    {Armature_mixamorig_Head}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.970663,-0.220637, 0.090348,-0.031152;;,
      1;4;-0.968798,-0.233780, 0.080525,-0.017098;;,
      2;4;-0.967110,-0.244442, 0.070212,-0.004039;;,
      3;4;-0.966561,-0.249277, 0.059736, 0.007153;;,
      4;4;-0.967920,-0.246393, 0.047276, 0.013663;;,
      5;4;-0.970957,-0.237076, 0.030015, 0.011671;;,
      6;4;-0.974102,-0.225973, 0.007885, 0.000124;;,
      7;4;-0.975587,-0.218530,-0.014409,-0.016365;;,
      8;4;-0.975168,-0.217202,-0.030678,-0.030495;;,
      9;4;-0.973872,-0.220469,-0.038526,-0.038512;;,
      10;4;-0.972641,-0.225511,-0.038426,-0.040467;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000002, 7.527311,-0.214792;;,
      1;3;-0.000002, 7.527330,-0.214811;;,
      2;3;-0.000001, 7.527326,-0.214800;;,
      3;3; 0.000002, 7.527326,-0.214804;;,
      4;3;-0.000002, 7.527318,-0.214805;;,
      5;3;-0.000004, 7.527333,-0.214822;;,
      6;3;-0.000001, 7.527338,-0.214818;;,
      7;3; 0.000001, 7.527324,-0.214812;;,
      8;3;-0.000001, 7.527321,-0.214802;;,
      9;3;-0.000001, 7.527311,-0.214821;;,
      10;3;-0.000001, 7.527333,-0.214810;;;
    }
  }
  Animation {
    {Armature_mixamorig_HeadTop_End}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      1;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      4;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000007,26.017372,-0.742499;;,
      1;3;-0.000005,26.017380,-0.742501;;,
      2;3;-0.000001,26.017370,-0.742500;;,
      3;3;-0.000002,26.017384,-0.742496;;,
      4;3;-0.000004,26.017378,-0.742498;;,
      5;3;-0.000001,26.017365,-0.742494;;,
      6;3;-0.000001,26.017370,-0.742500;;,
      7;3;-0.000001,26.017365,-0.742497;;,
      8;3; 0.000000,26.017380,-0.742493;;,
      9;3;-0.000001,26.017366,-0.742492;;,
      10;3;-0.000000,26.017389,-0.742494;;;
    }
  }
  Animation {
    {Armature_mixamorig_HeadTop_End_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      2;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      4;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000,26.027992,-0.000004;;,
      1;3; 0.000001,26.027977, 0.000001;;,
      2;3;-0.000002,26.027998,-0.000001;;,
      3;3; 0.000002,26.027987,-0.000001;;,
      4;3;-0.000001,26.028017, 0.000000;;,
      5;3;-0.000000,26.027990, 0.000004;;,
      6;3;-0.000001,26.027987,-0.000001;;,
      7;3;-0.000001,26.027994, 0.000000;;,
      8;3; 0.000001,26.028015, 0.000003;;,
      9;3;-0.000000,26.027977,-0.000004;;,
      10;3;-0.000000,26.027990,-0.000007;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftShoulder}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.391059, 0.641261, 0.293171,-0.591531;;,
      1;4;-0.371354, 0.639497, 0.286612,-0.609092;;,
      2;4;-0.338660, 0.644247, 0.272378,-0.629337;;,
      3;4;-0.314070, 0.640226, 0.269503,-0.647177;;,
      4;4;-0.296020, 0.633114, 0.275133,-0.660182;;,
      5;4;-0.283473, 0.624398, 0.288503,-0.668234;;,
      6;4;-0.273898, 0.613058, 0.308294,-0.673865;;,
      7;4;-0.267165, 0.602469, 0.330392,-0.675644;;,
      8;4;-0.262957, 0.594649, 0.348184,-0.675288;;,
      9;4;-0.266827, 0.593987, 0.358870,-0.668727;;,
      10;4;-0.263552, 0.598641, 0.358141,-0.666262;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 5.180532,11.652113, 0.054666;;,
      1;3; 5.180532,11.652103, 0.054665;;,
      2;3; 5.180532,11.652102, 0.054658;;,
      3;3; 5.180530,11.652116, 0.054668;;,
      4;3; 5.180532,11.652111, 0.054665;;,
      5;3; 5.180532,11.652104, 0.054664;;,
      6;3; 5.180533,11.652106, 0.054687;;,
      7;3; 5.180532,11.652121, 0.054676;;,
      8;3; 5.180532,11.652112, 0.054661;;,
      9;3; 5.180533,11.652115, 0.054681;;,
      10;3; 5.180532,11.652114, 0.054668;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftArm}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.985772, 0.080891,-0.102018,-0.106312;;,
      1;4;-0.985700, 0.101831,-0.082972,-0.105554;;,
      2;4;-0.985414, 0.129453,-0.055165,-0.095699;;,
      3;4;-0.983329, 0.157209,-0.034567,-0.084586;;,
      4;4;-0.980471, 0.184880,-0.016766,-0.064929;;,
      5;4;-0.976309, 0.214361, 0.003528,-0.029268;;,
      6;4;-0.968947, 0.245328, 0.025897, 0.016877;;,
      7;4;-0.958616, 0.273410, 0.045855, 0.064810;;,
      8;4;-0.948212, 0.294246, 0.058122, 0.104574;;,
      9;4;-0.942395, 0.300451, 0.059777, 0.134343;;,
      10;4;-0.939838, 0.305356, 0.043055, 0.147000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000006,10.993587,-0.000005;;,
      1;3;-0.000001,10.993594,-0.000008;;,
      2;3;-0.000001,10.993591, 0.000009;;,
      3;3; 0.000002,10.993599, 0.000009;;,
      4;3; 0.000004,10.993582, 0.000004;;,
      5;3; 0.000005,10.993584,-0.000006;;,
      6;3;-0.000010,10.993595, 0.000000;;,
      7;3; 0.000005,10.993585, 0.000002;;,
      8;3;-0.000005,10.993588,-0.000004;;,
      9;3; 0.000002,10.993585, 0.000011;;,
      10;3; 0.000009,10.993589,-0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftForeArm}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.922825, 0.055089,-0.250147, 0.287726;;,
      1;4;-0.925401, 0.049342,-0.215308, 0.307963;;,
      2;4;-0.929224, 0.044937,-0.181374, 0.318790;;,
      3;4;-0.926187, 0.037211,-0.152742, 0.342728;;,
      4;4;-0.918344, 0.026518,-0.134462, 0.371296;;,
      5;4;-0.904535, 0.011633,-0.131837, 0.405340;;,
      6;4;-0.885446,-0.005634,-0.138066, 0.443725;;,
      7;4;-0.866562,-0.019544,-0.148273, 0.476134;;,
      8;4;-0.855260,-0.027005,-0.154039, 0.494037;;,
      9;4;-0.864499,-0.020386,-0.150463, 0.479152;;,
      10;4;-0.881347,-0.022390,-0.126435, 0.454687;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000002,21.306484, 0.000002;;,
      1;3; 0.000005,21.306484,-0.000012;;,
      2;3; 0.000000,21.306492, 0.000000;;,
      3;3; 0.000006,21.306490,-0.000005;;,
      4;3; 0.000000,21.306480,-0.000001;;,
      5;3;-0.000003,21.306490, 0.000002;;,
      6;3; 0.000016,21.306496,-0.000002;;,
      7;3; 0.000002,21.306480,-0.000002;;,
      8;3;-0.000006,21.306494,-0.000005;;,
      9;3; 0.000003,21.306490,-0.000001;;,
      10;3; 0.000005,21.306475,-0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHand}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.978383, 0.086932,-0.006776, 0.187520;;,
      1;4;-0.976844, 0.089539,-0.006280, 0.194213;;,
      2;4;-0.978903, 0.071847,-0.007207, 0.191142;;,
      3;4;-0.974967, 0.085203,-0.005285, 0.205310;;,
      4;4;-0.970537, 0.099503,-0.004031, 0.219409;;,
      5;4;-0.970947, 0.094673,-0.003982, 0.219735;;,
      6;4;-0.973682, 0.080082,-0.004420, 0.213333;;,
      7;4;-0.976992, 0.061043,-0.005125, 0.204288;;,
      8;4;-0.979673, 0.045886,-0.005671, 0.195202;;,
      9;4;-0.981687, 0.038167,-0.005964, 0.186545;;,
      10;4;-0.983683, 0.046845,-0.005398, 0.173621;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000,24.281305,-0.000003;;,
      1;3; 0.000000,24.281298,-0.000000;;,
      2;3; 0.000000,24.281309,-0.000003;;,
      3;3; 0.000000,24.281298,-0.000003;;,
      4;3;-0.000000,24.281303, 0.000006;;,
      5;3; 0.000000,24.281303,-0.000000;;,
      6;3;-0.000001,24.281303, 0.000002;;,
      7;3;-0.000002,24.281292,-0.000002;;,
      8;3; 0.000002,24.281311,-0.000005;;,
      9;3;-0.000002,24.281300,-0.000008;;,
      10;3; 0.000000,24.281303, 0.000004;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      1;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      2;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      3;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      4;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      5;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      6;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      7;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      8;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      9;4;-0.958430, 0.088944, 0.126908, 0.239575;;,
      10;4;-0.958430, 0.088944, 0.126908, 0.239575;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 0.999999;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 0.999999, 1.000000, 0.999999;;,
      8;3; 0.999999, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-3.268705, 2.020260, 1.309101;;,
      1;3;-3.268703, 2.020258, 1.309104;;,
      2;3;-3.268710, 2.020259, 1.309102;;,
      3;3;-3.268707, 2.020255, 1.309101;;,
      4;3;-3.268710, 2.020270, 1.309104;;,
      5;3;-3.268710, 2.020267, 1.309100;;,
      6;3;-3.268714, 2.020266, 1.309096;;,
      7;3;-3.268706, 2.020276, 1.309102;;,
      8;3;-3.268701, 2.020268, 1.309097;;,
      9;3;-3.268709, 2.020267, 1.309098;;,
      10;3;-3.268712, 2.020260, 1.309106;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      1;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      2;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      3;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      4;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      5;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      6;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      7;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      8;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      9;4;-0.992157, 0.111568, 0.046746,-0.031498;;,
      10;4;-0.992157, 0.111568, 0.046746,-0.031498;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.446724, 3.216763,-0.000015;;,
      1;3;-0.446719, 3.216757,-0.000010;;,
      2;3;-0.446725, 3.216760,-0.000011;;,
      3;3;-0.446722, 3.216762,-0.000010;;,
      4;3;-0.446725, 3.216762,-0.000009;;,
      5;3;-0.446725, 3.216761,-0.000009;;,
      6;3;-0.446719, 3.216766,-0.000008;;,
      7;3;-0.446719, 3.216760,-0.000009;;,
      8;3;-0.446713, 3.216761,-0.000000;;,
      9;3;-0.446718, 3.216757,-0.000003;;,
      10;3;-0.446725, 3.216760,-0.000012;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      1;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      2;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      3;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      4;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      5;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      6;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      7;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      8;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      9;4;-0.892228, 0.154886, 0.424158,-0.005416;;,
      10;4;-0.892228, 0.154886, 0.424158,-0.005416;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.289039, 4.061817, 0.000008;;,
      1;3; 0.289029, 4.061818, 0.000003;;,
      2;3; 0.289036, 4.061812,-0.000007;;,
      3;3; 0.289030, 4.061812,-0.000001;;,
      4;3; 0.289025, 4.061812,-0.000003;;,
      5;3; 0.289030, 4.061810,-0.000008;;,
      6;3; 0.289035, 4.061809,-0.000002;;,
      7;3; 0.289034, 4.061813,-0.000004;;,
      8;3; 0.289032, 4.061814,-0.000002;;,
      9;3; 0.289016, 4.061805,-0.000004;;,
      10;3; 0.289031, 4.061810,-0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      3;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      8;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000001;;,
      1;3; 1.000000, 1.000000, 1.000001;;,
      2;3; 1.000000, 1.000000, 1.000001;;,
      3;3; 1.000000, 1.000000, 1.000001;;,
      4;3; 1.000000, 1.000000, 1.000001;;,
      5;3; 1.000000, 1.000000, 1.000001;;,
      6;3; 1.000000, 1.000001, 1.000001;;,
      7;3; 1.000000, 1.000001, 1.000001;;,
      8;3; 1.000000, 1.000001, 1.000001;;,
      9;3; 1.000000, 1.000001, 1.000001;;,
      10;3; 1.000000, 1.000001, 1.000001;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.157710, 3.553952,-0.000024;;,
      1;3; 0.157712, 3.553961,-0.000022;;,
      2;3; 0.157701, 3.553954,-0.000028;;,
      3;3; 0.157708, 3.553958,-0.000026;;,
      4;3; 0.157711, 3.553959,-0.000024;;,
      5;3; 0.157705, 3.553959,-0.000019;;,
      6;3; 0.157704, 3.553965,-0.000026;;,
      7;3; 0.157701, 3.553955,-0.000027;;,
      8;3; 0.157701, 3.553956,-0.000027;;,
      9;3; 0.157705, 3.553960,-0.000020;;,
      10;3; 0.157702, 3.553954,-0.000020;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandThumb4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      8;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000002, 3.557465, 0.000009;;,
      1;3;-0.000002, 3.557468,-0.000004;;,
      2;3; 0.000003, 3.557469, 0.000004;;,
      3;3; 0.000000, 3.557472,-0.000004;;,
      4;3; 0.000001, 3.557472, 0.000005;;,
      5;3;-0.000002, 3.557465,-0.000003;;,
      6;3;-0.000000, 3.557468,-0.000001;;,
      7;3;-0.000001, 3.557467,-0.000004;;,
      8;3; 0.000002, 3.557478, 0.000002;;,
      9;3;-0.000001, 3.557467, 0.000003;;,
      10;3; 0.000002, 3.557460,-0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      1;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      2;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      3;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      4;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      5;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      6;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      7;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      8;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      9;4;-0.934907, 0.335077, 0.000963, 0.116923;;,
      10;4;-0.934907, 0.335077, 0.000963, 0.116923;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-4.000833,10.644614, 0.188338;;,
      1;3;-4.000832,10.644612, 0.188332;;,
      2;3;-4.000840,10.644615, 0.188336;;,
      3;3;-4.000837,10.644612, 0.188333;;,
      4;3;-4.000839,10.644621, 0.188335;;,
      5;3;-4.000837,10.644621, 0.188332;;,
      6;3;-4.000840,10.644624, 0.188327;;,
      7;3;-4.000842,10.644622, 0.188335;;,
      8;3;-4.000834,10.644616, 0.188326;;,
      9;3;-4.000839,10.644613, 0.188326;;,
      10;3;-4.000837,10.644613, 0.188333;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      1;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      2;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      3;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      4;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      5;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      6;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      7;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      8;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      9;4;-0.932393, 0.359262, 0.013837, 0.037191;;,
      10;4;-0.932393, 0.359262, 0.013837, 0.037191;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000001, 1.000000;;,
      1;3; 1.000000, 1.000001, 1.000000;;,
      2;3; 1.000000, 1.000001, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000045, 3.442457,-0.000016;;,
      1;3;-0.000051, 3.442450,-0.000023;;,
      2;3;-0.000051, 3.442446,-0.000020;;,
      3;3;-0.000062, 3.442451,-0.000021;;,
      4;3;-0.000055, 3.442444,-0.000020;;,
      5;3;-0.000055, 3.442456,-0.000021;;,
      6;3;-0.000056, 3.442451,-0.000019;;,
      7;3;-0.000049, 3.442450,-0.000021;;,
      8;3;-0.000060, 3.442446,-0.000020;;,
      9;3;-0.000041, 3.442453,-0.000021;;,
      10;3;-0.000049, 3.442461,-0.000020;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      1;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      2;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      3;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      4;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      5;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      6;4;-0.998347, 0.054995, 0.001058,-0.016690;;,
      7;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      8;4;-0.998347, 0.054995, 0.001059,-0.016690;;,
      9;4;-0.998347, 0.054995, 0.001058,-0.016690;;,
      10;4;-0.998347, 0.054995, 0.001059,-0.016690;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 0.999999, 1.000000;;,
      1;3; 1.000000, 0.999999, 0.999999;;,
      2;3; 1.000000, 0.999999, 0.999999;;,
      3;3; 1.000000, 0.999999, 1.000000;;,
      4;3; 1.000000, 0.999999, 0.999999;;,
      5;3; 1.000000, 0.999999, 1.000000;;,
      6;3; 1.000000, 0.999999, 0.999999;;,
      7;3; 1.000000, 0.999999, 0.999999;;,
      8;3; 1.000000, 0.999999, 1.000000;;,
      9;3; 1.000000, 0.999999, 1.000000;;,
      10;3; 1.000000, 0.999999, 0.999999;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000150, 3.228660, 0.000035;;,
      1;3;-0.000163, 3.228653, 0.000025;;,
      2;3;-0.000165, 3.228656, 0.000033;;,
      3;3;-0.000152, 3.228658, 0.000034;;,
      4;3;-0.000159, 3.228658, 0.000040;;,
      5;3;-0.000152, 3.228657, 0.000028;;,
      6;3;-0.000150, 3.228658, 0.000025;;,
      7;3;-0.000149, 3.228660, 0.000037;;,
      8;3;-0.000150, 3.228659, 0.000027;;,
      9;3;-0.000158, 3.228665, 0.000025;;,
      10;3;-0.000159, 3.228664, 0.000035;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      4;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      8;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000001, 1.000000;;,
      1;3; 1.000000, 1.000001, 1.000000;;,
      2;3; 1.000000, 1.000001, 1.000000;;,
      3;3; 1.000000, 1.000001, 1.000000;;,
      4;3; 1.000000, 1.000001, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000001, 1.000000;;,
      7;3; 1.000000, 1.000001, 1.000000;;,
      8;3; 1.000000, 1.000001, 1.000000;;,
      9;3; 1.000000, 1.000001, 1.000000;;,
      10;3; 1.000000, 1.000001, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000215, 2.712049,-0.000008;;,
      1;3; 0.000230, 2.712050,-0.000007;;,
      2;3; 0.000222, 2.712054,-0.000009;;,
      3;3; 0.000214, 2.712050,-0.000009;;,
      4;3; 0.000216, 2.712046,-0.000009;;,
      5;3; 0.000206, 2.712045,-0.000003;;,
      6;3; 0.000215, 2.712041,-0.000002;;,
      7;3; 0.000216, 2.712039, 0.000000;;,
      8;3; 0.000216, 2.712044,-0.000003;;,
      9;3; 0.000211, 2.712045,-0.000009;;,
      10;3; 0.000202, 2.712040, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandIndex4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      6;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000001, 2.712037, 0.000000;;,
      1;3;-0.000006, 2.712035, 0.000000;;,
      2;3; 0.000001, 2.712036,-0.000002;;,
      3;3;-0.000007, 2.712036,-0.000005;;,
      4;3; 0.000004, 2.712035, 0.000006;;,
      5;3;-0.000003, 2.712036, 0.000004;;,
      6;3;-0.000002, 2.712036, 0.000002;;,
      7;3;-0.000004, 2.712036,-0.000006;;,
      8;3;-0.000003, 2.712037,-0.000006;;,
      9;3; 0.000002, 2.712037,-0.000006;;,
      10;3; 0.000012, 2.712036,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      1;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      2;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      3;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      4;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      5;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      6;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      7;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      8;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      9;4;-0.938462, 0.336966, 0.020288, 0.073018;;,
      10;4;-0.938462, 0.336966, 0.020288, 0.073018;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-1.145570,10.269861, 0.098122;;,
      1;3;-1.145574,10.269859, 0.098124;;,
      2;3;-1.145579,10.269859, 0.098128;;,
      3;3;-1.145577,10.269857, 0.098123;;,
      4;3;-1.145580,10.269865, 0.098127;;,
      5;3;-1.145580,10.269864, 0.098123;;,
      6;3;-1.145586,10.269857, 0.098118;;,
      7;3;-1.145579,10.269867, 0.098123;;,
      8;3;-1.145567,10.269861, 0.098119;;,
      9;3;-1.145578,10.269858, 0.098118;;,
      10;3;-1.145582,10.269846, 0.098126;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      1;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      2;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      3;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      4;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      5;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      6;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      7;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      8;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      9;4;-0.902533, 0.428248, 0.017938, 0.041419;;,
      10;4;-0.902533, 0.428248, 0.017938, 0.041419;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 0.999999, 0.999999;;,
      3;3; 1.000000, 0.999999, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 0.999999, 1.000000;;,
      8;3; 1.000000, 1.000000, 0.999999;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.001398, 4.567478, 0.000003;;,
      1;3;-0.001394, 4.567485,-0.000004;;,
      2;3;-0.001397, 4.567488, 0.000003;;,
      3;3;-0.001390, 4.567478,-0.000005;;,
      4;3;-0.001397, 4.567482,-0.000008;;,
      5;3;-0.001392, 4.567484,-0.000005;;,
      6;3;-0.001397, 4.567485,-0.000006;;,
      7;3;-0.001400, 4.567476,-0.000003;;,
      8;3;-0.001403, 4.567483,-0.000004;;,
      9;3;-0.001399, 4.567473,-0.000007;;,
      10;3;-0.001405, 4.567479,-0.000009;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      1;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      2;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      3;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      4;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      5;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      6;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      7;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      8;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      9;4;-0.986881, 0.160893, 0.002677,-0.013121;;,
      10;4;-0.986881, 0.160893, 0.002677,-0.013121;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000001;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000001;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.001003, 4.214485, 0.000012;;,
      1;3; 0.001009, 4.214486, 0.000008;;,
      2;3; 0.001009, 4.214484, 0.000006;;,
      3;3; 0.001003, 4.214488, 0.000011;;,
      4;3; 0.001008, 4.214488, 0.000011;;,
      5;3; 0.001002, 4.214489, 0.000003;;,
      6;3; 0.001000, 4.214487, 0.000013;;,
      7;3; 0.001005, 4.214490, 0.000007;;,
      8;3; 0.001001, 4.214484, 0.000014;;,
      9;3; 0.000994, 4.214487, 0.000004;;,
      10;3; 0.000994, 4.214488, 0.000009;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      2;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      4;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      5;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      6;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      7;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      8;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      9;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      10;4;-1.000000, 0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000402, 3.872929,-0.000029;;,
      1;3; 0.000402, 3.872930,-0.000027;;,
      2;3; 0.000401, 3.872930,-0.000025;;,
      3;3; 0.000405, 3.872929,-0.000034;;,
      4;3; 0.000402, 3.872934,-0.000026;;,
      5;3; 0.000400, 3.872931,-0.000029;;,
      6;3; 0.000400, 3.872931,-0.000031;;,
      7;3; 0.000401, 3.872929,-0.000037;;,
      8;3; 0.000397, 3.872928,-0.000026;;,
      9;3; 0.000403, 3.872931,-0.000032;;,
      10;3; 0.000400, 3.872930,-0.000037;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandMiddle4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      1;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      2;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      4;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      6;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      7;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      8;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      9;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      10;4;-1.000000, 0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000004, 3.872905,-0.000010;;,
      1;3;-0.000001, 3.872908,-0.000005;;,
      2;3;-0.000003, 3.872904,-0.000006;;,
      3;3;-0.000000, 3.872907,-0.000003;;,
      4;3; 0.000003, 3.872907,-0.000004;;,
      5;3;-0.000002, 3.872910, 0.000002;;,
      6;3;-0.000001, 3.872909, 0.000002;;,
      7;3; 0.000002, 3.872904,-0.000000;;,
      8;3; 0.000000, 3.872907, 0.000002;;,
      9;3; 0.000006, 3.872906,-0.000000;;,
      10;3; 0.000005, 3.872907,-0.000005;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      1;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      2;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      3;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      4;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      5;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      6;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      7;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      8;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      9;4;-0.850389, 0.523103, 0.024688, 0.050921;;,
      10;4;-0.850389, 0.523103, 0.024688, 0.050921;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 1.511721,10.276354, 0.094536;;,
      1;3; 1.511714,10.276351, 0.094531;;,
      2;3; 1.511714,10.276359, 0.094539;;,
      3;3; 1.511717,10.276352, 0.094533;;,
      4;3; 1.511709,10.276357, 0.094538;;,
      5;3; 1.511708,10.276352, 0.094533;;,
      6;3; 1.511709,10.276359, 0.094531;;,
      7;3; 1.511710,10.276361, 0.094535;;,
      8;3; 1.511721,10.276354, 0.094532;;,
      9;3; 1.511705,10.276351, 0.094531;;,
      10;3; 1.511706,10.276342, 0.094540;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      1;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      2;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      3;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      4;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      5;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      6;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      7;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      8;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      9;4;-0.925418, 0.378702,-0.003710, 0.013144;;,
      10;4;-0.925418, 0.378702,-0.003710, 0.013144;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 0.999999;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 0.999999, 0.999999;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000589, 3.979047,-0.000057;;,
      1;3;-0.000591, 3.979053,-0.000056;;,
      2;3;-0.000596, 3.979047,-0.000059;;,
      3;3;-0.000594, 3.979052,-0.000056;;,
      4;3;-0.000594, 3.979053,-0.000055;;,
      5;3;-0.000590, 3.979049,-0.000051;;,
      6;3;-0.000587, 3.979047,-0.000045;;,
      7;3;-0.000597, 3.979043,-0.000046;;,
      8;3;-0.000591, 3.979050,-0.000047;;,
      9;3;-0.000596, 3.979043,-0.000052;;,
      10;3;-0.000597, 3.979047,-0.000046;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      1;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      2;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      3;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      4;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      5;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      6;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      7;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      8;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      9;4;-0.990596, 0.136469,-0.001928,-0.009552;;,
      10;4;-0.990596, 0.136469,-0.001928,-0.009552;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000087, 3.715582, 0.000046;;,
      1;3; 0.000085, 3.715585, 0.000051;;,
      2;3; 0.000090, 3.715585, 0.000052;;,
      3;3; 0.000093, 3.715588, 0.000051;;,
      4;3; 0.000087, 3.715587, 0.000049;;,
      5;3; 0.000089, 3.715587, 0.000047;;,
      6;3; 0.000093, 3.715588, 0.000042;;,
      7;3; 0.000092, 3.715586, 0.000056;;,
      8;3; 0.000098, 3.715586, 0.000049;;,
      9;3; 0.000095, 3.715590, 0.000046;;,
      10;3; 0.000093, 3.715590, 0.000052;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      7;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000501, 3.315392,-0.000008;;,
      1;3; 0.000497, 3.315396, 0.000000;;,
      2;3; 0.000500, 3.315392,-0.000004;;,
      3;3; 0.000506, 3.315391,-0.000003;;,
      4;3; 0.000500, 3.315391, 0.000002;;,
      5;3; 0.000500, 3.315389,-0.000006;;,
      6;3; 0.000496, 3.315387,-0.000001;;,
      7;3; 0.000495, 3.315389, 0.000002;;,
      8;3; 0.000493, 3.315387, 0.000012;;,
      9;3; 0.000492, 3.315385, 0.000009;;,
      10;3; 0.000483, 3.315382, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandRing4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      2;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      3;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      4;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      8;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      9;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      10;4;-1.000000, 0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000003, 3.315384,-0.000001;;,
      1;3; 0.000004, 3.315385,-0.000007;;,
      2;3; 0.000000, 3.315388,-0.000003;;,
      3;3; 0.000006, 3.315383,-0.000003;;,
      4;3; 0.000002, 3.315383, 0.000002;;,
      5;3; 0.000005, 3.315383, 0.000010;;,
      6;3;-0.000000, 3.315383,-0.000003;;,
      7;3; 0.000005, 3.315386, 0.000003;;,
      8;3; 0.000007, 3.315386,-0.000008;;,
      9;3;-0.000001, 3.315383, 0.000000;;,
      10;3; 0.000003, 3.315383, 0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      1;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      2;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      3;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      4;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      5;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      6;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      7;4;-0.885926, 0.447995, 0.007113, 0.119940;;,
      8;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      9;4;-0.885926, 0.447995, 0.007112, 0.119940;;,
      10;4;-0.885926, 0.447995, 0.007113, 0.119940;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 3.634718,10.279571, 0.214919;;,
      1;3; 3.634713,10.279566, 0.214916;;,
      2;3; 3.634707,10.279564, 0.214913;;,
      3;3; 3.634713,10.279570, 0.214914;;,
      4;3; 3.634710,10.279576, 0.214917;;,
      5;3; 3.634705,10.279573, 0.214911;;,
      6;3; 3.634707,10.279575, 0.214910;;,
      7;3; 3.634706,10.279576, 0.214913;;,
      8;3; 3.634718,10.279572, 0.214909;;,
      9;3; 3.634707,10.279570, 0.214907;;,
      10;3; 3.634707,10.279562, 0.214912;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      1;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      2;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      3;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      4;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      5;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      6;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      7;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      8;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      9;4;-0.802061, 0.597095, 0.000059, 0.013279;;,
      10;4;-0.802061, 0.597095, 0.000059, 0.013279;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.001719, 3.272577,-0.000041;;,
      1;3; 0.001723, 3.272580,-0.000042;;,
      2;3; 0.001716, 3.272568,-0.000038;;,
      3;3; 0.001713, 3.272577,-0.000038;;,
      4;3; 0.001715, 3.272571,-0.000031;;,
      5;3; 0.001718, 3.272572,-0.000040;;,
      6;3; 0.001712, 3.272570,-0.000038;;,
      7;3; 0.001710, 3.272577,-0.000037;;,
      8;3; 0.001716, 3.272577,-0.000038;;,
      9;3; 0.001710, 3.272574,-0.000040;;,
      10;3; 0.001713, 3.272568,-0.000038;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      1;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      2;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      3;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      4;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      5;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      6;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      7;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      8;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      9;4;-0.967134, 0.254192, 0.000095, 0.006100;;,
      10;4;-0.967134, 0.254192, 0.000095, 0.006100;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 0.999999;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 0.999999;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 0.999999;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.001550, 2.574705,-0.000007;;,
      1;3;-0.001548, 2.574709,-0.000007;;,
      2;3;-0.001557, 2.574712,-0.000008;;,
      3;3;-0.001553, 2.574711,-0.000007;;,
      4;3;-0.001557, 2.574714,-0.000007;;,
      5;3;-0.001550, 2.574715,-0.000012;;,
      6;3;-0.001550, 2.574714,-0.000007;;,
      7;3;-0.001550, 2.574712,-0.000016;;,
      8;3;-0.001552, 2.574715,-0.000011;;,
      9;3;-0.001550, 2.574713,-0.000014;;,
      10;3;-0.001547, 2.574717,-0.000015;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      4;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      5;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      6;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      7;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      9;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000172, 2.249532, 0.000013;;,
      1;3;-0.000160, 2.249531, 0.000009;;,
      2;3;-0.000171, 2.249538, 0.000019;;,
      3;3;-0.000164, 2.249537, 0.000021;;,
      4;3;-0.000163, 2.249539, 0.000018;;,
      5;3;-0.000160, 2.249534, 0.000018;;,
      6;3;-0.000161, 2.249541, 0.000024;;,
      7;3;-0.000169, 2.249538, 0.000021;;,
      8;3;-0.000165, 2.249537, 0.000020;;,
      9;3;-0.000158, 2.249537, 0.000027;;,
      10;3;-0.000162, 2.249540, 0.000027;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftHandPinky4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000003, 2.249514,-0.000007;;,
      1;3; 0.000010, 2.249505,-0.000003;;,
      2;3;-0.000000, 2.249510, 0.000007;;,
      3;3;-0.000004, 2.249509, 0.000005;;,
      4;3; 0.000003, 2.249509,-0.000001;;,
      5;3;-0.000005, 2.249506, 0.000003;;,
      6;3;-0.000005, 2.249510,-0.000005;;,
      7;3;-0.000004, 2.249505,-0.000004;;,
      8;3;-0.000001, 2.249513,-0.000005;;,
      9;3;-0.000004, 2.249503, 0.000004;;,
      10;3; 0.000002, 2.249508, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightShoulder}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.458090, 0.578217,-0.432578, 0.518358;;,
      1;4;-0.446596, 0.582018,-0.424968, 0.530292;;,
      2;4;-0.436711, 0.586191,-0.417053, 0.540121;;,
      3;4;-0.428788, 0.590670,-0.409089, 0.547628;;,
      4;4;-0.423174, 0.595398,-0.401331, 0.552592;;,
      5;4;-0.420195, 0.600329,-0.394041, 0.554773;;,
      6;4;-0.420143, 0.605422,-0.387490, 0.553891;;,
      7;4;-0.425604, 0.611035,-0.382026, 0.547314;;,
      8;4;-0.436547, 0.617246,-0.377463, 0.534749;;,
      9;4;-0.449375, 0.623602,-0.373265, 0.519477;;,
      10;4;-0.460774, 0.629703,-0.368856, 0.505082;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-5.180532,11.651790, 0.039934;;,
      1;3;-5.180533,11.651771, 0.039943;;,
      2;3;-5.180530,11.651773, 0.039937;;,
      3;3;-5.180537,11.651786, 0.039948;;,
      4;3;-5.180534,11.651785, 0.039949;;,
      5;3;-5.180532,11.651777, 0.039944;;,
      6;3;-5.180531,11.651785, 0.039957;;,
      7;3;-5.180532,11.651785, 0.039951;;,
      8;3;-5.180532,11.651787, 0.039942;;,
      9;3;-5.180533,11.651778, 0.039951;;,
      10;3;-5.180532,11.651780, 0.039952;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightArm}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.991775, 0.092398,-0.086998,-0.016618;;,
      1;4;-0.987131, 0.126535,-0.076693,-0.060664;;,
      2;4;-0.981023, 0.158345,-0.057079,-0.096246;;,
      3;4;-0.972896, 0.189519,-0.031178,-0.128777;;,
      4;4;-0.963830, 0.215303,-0.001491,-0.157082;;,
      5;4;-0.955639, 0.231476, 0.030038,-0.179643;;,
      6;4;-0.950296, 0.236701, 0.061481,-0.192691;;,
      7;4;-0.943306, 0.239123, 0.097042,-0.208751;;,
      8;4;-0.931888, 0.251937, 0.134774,-0.223492;;,
      9;4;-0.922805, 0.273095, 0.163084,-0.217377;;,
      10;4;-0.920793, 0.288348, 0.183265,-0.188172;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000007,10.993587, 0.000011;;,
      1;3;-0.000006,10.993590,-0.000002;;,
      2;3;-0.000002,10.993590, 0.000007;;,
      3;3; 0.000007,10.993589,-0.000005;;,
      4;3; 0.000002,10.993590, 0.000000;;,
      5;3;-0.000001,10.993589, 0.000005;;,
      6;3; 0.000002,10.993590, 0.000002;;,
      7;3;-0.000003,10.993587,-0.000004;;,
      8;3; 0.000005,10.993589, 0.000012;;,
      9;3; 0.000005,10.993592,-0.000007;;,
      10;3;-0.000010,10.993592,-0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightForeArm}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.738387, 0.138501, 0.203963,-0.627696;;,
      1;4;-0.732353, 0.121272, 0.196290,-0.640642;;,
      2;4;-0.731245, 0.101746, 0.186712,-0.648125;;,
      3;4;-0.735090, 0.081124, 0.176244,-0.649616;;,
      4;4;-0.739862, 0.061031, 0.165428,-0.649240;;,
      5;4;-0.741670, 0.043440, 0.154472,-0.651289;;,
      6;4;-0.738319, 0.030537, 0.143461,-0.658310;;,
      7;4;-0.738187, 0.024599, 0.131385,-0.661221;;,
      8;4;-0.743456, 0.024449, 0.118011,-0.657836;;,
      9;4;-0.746300, 0.025986, 0.104892,-0.656779;;,
      10;4;-0.744321, 0.027515, 0.093316,-0.660698;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 0.999999;;,
      8;3; 1.000000, 1.000000, 0.999999;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 0.999999;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000012,21.306309, 0.000008;;,
      1;3; 0.000008,21.306297,-0.000004;;,
      2;3; 0.000006,21.306305,-0.000008;;,
      3;3;-0.000000,21.306299,-0.000003;;,
      4;3; 0.000006,21.306303, 0.000004;;,
      5;3; 0.000003,21.306305,-0.000005;;,
      6;3;-0.000000,21.306299,-0.000000;;,
      7;3;-0.000005,21.306293, 0.000000;;,
      8;3;-0.000001,21.306303, 0.000002;;,
      9;3; 0.000006,21.306293, 0.000003;;,
      10;3; 0.000000,21.306290, 0.000007;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHand}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.999610, 0.011701, 0.023374, 0.009834;;,
      1;4;-0.999404, 0.015872, 0.022587, 0.020743;;,
      2;4;-0.999155, 0.020342, 0.020916, 0.028964;;,
      3;4;-0.998928, 0.025345, 0.018545, 0.033996;;,
      4;4;-0.998769, 0.031091, 0.015676, 0.035335;;,
      5;4;-0.998680, 0.037776, 0.012535, 0.032480;;,
      6;4;-0.998606, 0.045576, 0.009390, 0.024925;;,
      7;4;-0.998419, 0.055191, 0.006113, 0.008685;;,
      8;4;-0.997658, 0.066341, 0.002878,-0.016404;;,
      9;4;-0.995950, 0.077795, 0.000242,-0.045069;;,
      10;4;-0.993467, 0.088519,-0.001617,-0.072005;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000001;;,
      1;3; 1.000000, 0.999999, 1.000000;;,
      2;3; 1.000000, 0.999999, 1.000000;;,
      3;3; 1.000000, 0.999999, 1.000000;;,
      4;3; 1.000000, 0.999999, 1.000001;;,
      5;3; 1.000000, 0.999999, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 0.999999, 1.000001;;,
      8;3; 1.000000, 0.999999, 1.000001;;,
      9;3; 1.000000, 0.999999, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000001;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000009,24.291656,-0.000001;;,
      1;3; 0.000007,24.291658,-0.000005;;,
      2;3;-0.000001,24.291660,-0.000011;;,
      3;3;-0.000009,24.291666, 0.000002;;,
      4;3; 0.000001,24.291655,-0.000002;;,
      5;3; 0.000017,24.291658, 0.000003;;,
      6;3; 0.000008,24.291658,-0.000002;;,
      7;3;-0.000008,24.291658,-0.000003;;,
      8;3; 0.000010,24.291660,-0.000003;;,
      9;3;-0.000005,24.291666, 0.000000;;,
      10;3; 0.000000,24.291656,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      1;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      2;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      3;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      4;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      5;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      6;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      7;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      8;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      9;4;-0.878465, 0.436986,-0.193096,-0.007530;;,
      10;4;-0.878465, 0.436986,-0.193096,-0.007530;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 3.351191, 2.208719, 1.381192;;,
      1;3; 3.351191, 2.208732, 1.381196;;,
      2;3; 3.351191, 2.208718, 1.381191;;,
      3;3; 3.351176, 2.208731, 1.381202;;,
      4;3; 3.351190, 2.208729, 1.381197;;,
      5;3; 3.351181, 2.208730, 1.381197;;,
      6;3; 3.351192, 2.208732, 1.381197;;,
      7;3; 3.351182, 2.208741, 1.381197;;,
      8;3; 3.351173, 2.208738, 1.381197;;,
      9;3; 3.351185, 2.208745, 1.381202;;,
      10;3; 3.351190, 2.208743, 1.381203;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      1;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      2;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      3;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      4;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      5;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      6;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      7;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      8;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      9;4;-0.979212,-0.108883,-0.150861,-0.080807;;,
      10;4;-0.979212,-0.108883,-0.150861,-0.080807;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.503309, 3.358190, 0.000004;;,
      1;3; 0.503305, 3.358182, 0.000004;;,
      2;3; 0.503294, 3.358191, 0.000000;;,
      3;3; 0.503297, 3.358195, 0.000000;;,
      4;3; 0.503299, 3.358199,-0.000000;;,
      5;3; 0.503296, 3.358207, 0.000009;;,
      6;3; 0.503295, 3.358207, 0.000004;;,
      7;3; 0.503299, 3.358200, 0.000004;;,
      8;3; 0.503286, 3.358195, 0.000004;;,
      9;3; 0.503286, 3.358198, 0.000008;;,
      10;3; 0.503287, 3.358197, 0.000005;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      1;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      2;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      3;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      4;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      5;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      6;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      7;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      8;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      9;4;-0.979809,-0.129377, 0.133979, 0.072698;;,
      10;4;-0.979809,-0.129377, 0.133979, 0.072698;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.013919, 3.929758, 0.000009;;,
      1;3;-0.013913, 3.929773,-0.000002;;,
      2;3;-0.013913, 3.929750,-0.000002;;,
      3;3;-0.013914, 3.929754, 0.000010;;,
      4;3;-0.013903, 3.929751,-0.000007;;,
      5;3;-0.013902, 3.929754,-0.000003;;,
      6;3;-0.013907, 3.929749,-0.000004;;,
      7;3;-0.013903, 3.929749, 0.000010;;,
      8;3;-0.013902, 3.929744,-0.000008;;,
      9;3;-0.013909, 3.929744, 0.000000;;,
      10;3;-0.013890, 3.929743,-0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 0.999999, 1.000000, 1.000000;;,
      2;3; 0.999999, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.489386, 3.302343, 0.000038;;,
      1;3;-0.489382, 3.302346, 0.000044;;,
      2;3;-0.489380, 3.302344, 0.000032;;,
      3;3;-0.489372, 3.302325, 0.000027;;,
      4;3;-0.489375, 3.302330, 0.000031;;,
      5;3;-0.489372, 3.302331, 0.000027;;,
      6;3;-0.489390, 3.302338, 0.000033;;,
      7;3;-0.489385, 3.302329, 0.000029;;,
      8;3;-0.489383, 3.302330, 0.000033;;,
      9;3;-0.489389, 3.302331, 0.000033;;,
      10;3;-0.489374, 3.302327, 0.000033;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandThumb4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000, 3.338408, 0.000006;;,
      1;3; 0.000005, 3.338410,-0.000004;;,
      2;3; 0.000004, 3.338410, 0.000004;;,
      3;3;-0.000010, 3.338414, 0.000004;;,
      4;3;-0.000004, 3.338412,-0.000004;;,
      5;3;-0.000008, 3.338405,-0.000003;;,
      6;3;-0.000004, 3.338407, 0.000001;;,
      7;3;-0.000005, 3.338407,-0.000001;;,
      8;3;-0.000004, 3.338409, 0.000005;;,
      9;3; 0.000003, 3.338407, 0.000001;;,
      10;3;-0.000002, 3.338405, 0.000009;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      1;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      2;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      3;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      4;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      5;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      6;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      7;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      8;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      9;4;-0.859940, 0.509042,-0.014954,-0.034011;;,
      10;4;-0.859940, 0.509042,-0.014954,-0.034011;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 0.999999, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 0.999999, 1.000000, 1.000000;;,
      8;3; 0.999999, 1.000000, 1.000000;;,
      9;3; 0.999999, 1.000000, 1.000000;;,
      10;3; 0.999999, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 3.876478,10.479907, 0.248460;;,
      1;3; 3.876482,10.479915, 0.248461;;,
      2;3; 3.876488,10.479906, 0.248462;;,
      3;3; 3.876476,10.479910, 0.248468;;,
      4;3; 3.876487,10.479915, 0.248468;;,
      5;3; 3.876476,10.479913, 0.248466;;,
      6;3; 3.876495,10.479910, 0.248469;;,
      7;3; 3.876492,10.479917, 0.248466;;,
      8;3; 3.876485,10.479912, 0.248465;;,
      9;3; 3.876496,10.479912, 0.248471;;,
      10;3; 3.876497,10.479914, 0.248472;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      1;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      2;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      3;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      4;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      5;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      6;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      7;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      8;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      9;4;-0.835138, 0.548317,-0.021221,-0.037986;;,
      10;4;-0.835138, 0.548317,-0.021221,-0.037986;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000001, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000001, 1.000000, 1.000000;;,
      5;3; 1.000001, 1.000000, 1.000000;;,
      6;3; 1.000001, 1.000000, 1.000000;;,
      7;3; 1.000001, 1.000000, 1.000000;;,
      8;3; 1.000001, 1.000000, 1.000000;;,
      9;3; 1.000001, 1.000000, 1.000000;;,
      10;3; 1.000001, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.001575, 3.405115, 0.000013;;,
      1;3; 0.001575, 3.405110, 0.000018;;,
      2;3; 0.001568, 3.405120, 0.000013;;,
      3;3; 0.001566, 3.405118, 0.000020;;,
      4;3; 0.001572, 3.405110, 0.000015;;,
      5;3; 0.001566, 3.405120, 0.000011;;,
      6;3; 0.001575, 3.405113, 0.000024;;,
      7;3; 0.001571, 3.405108, 0.000011;;,
      8;3; 0.001562, 3.405115, 0.000020;;,
      9;3; 0.001577, 3.405117, 0.000013;;,
      10;3; 0.001574, 3.405114, 0.000020;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      1;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      2;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      3;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      4;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      5;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      6;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      7;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      8;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      9;4;-0.868290, 0.495859,-0.012561, 0.006149;;,
      10;4;-0.868290, 0.495859,-0.012561, 0.006149;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000139, 3.328513,-0.000002;;,
      1;3;-0.000141, 3.328512,-0.000014;;,
      2;3;-0.000152, 3.328506,-0.000020;;,
      3;3;-0.000138, 3.328506,-0.000004;;,
      4;3;-0.000141, 3.328507,-0.000017;;,
      5;3;-0.000131, 3.328508,-0.000008;;,
      6;3;-0.000139, 3.328505,-0.000010;;,
      7;3;-0.000143, 3.328511,-0.000009;;,
      8;3;-0.000140, 3.328510,-0.000006;;,
      9;3;-0.000123, 3.328513,-0.000008;;,
      10;3;-0.000145, 3.328505,-0.000010;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      4;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      7;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      8;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      9;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.001418, 2.857780,-0.000038;;,
      1;3;-0.001415, 2.857775,-0.000023;;,
      2;3;-0.001416, 2.857783,-0.000021;;,
      3;3;-0.001412, 2.857776,-0.000024;;,
      4;3;-0.001415, 2.857771,-0.000019;;,
      5;3;-0.001407, 2.857770,-0.000023;;,
      6;3;-0.001415, 2.857780,-0.000025;;,
      7;3;-0.001403, 2.857775,-0.000010;;,
      8;3;-0.001408, 2.857779,-0.000012;;,
      9;3;-0.001405, 2.857778,-0.000013;;,
      10;3;-0.001411, 2.857764,-0.000019;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandIndex4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      3;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      4;4;-1.000000,-0.000000,-0.000000,-0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000005, 2.857757, 0.000001;;,
      1;3;-0.000001, 2.857751, 0.000002;;,
      2;3;-0.000005, 2.857752,-0.000007;;,
      3;3;-0.000007, 2.857754, 0.000005;;,
      4;3; 0.000009, 2.857761, 0.000006;;,
      5;3;-0.000001, 2.857753, 0.000004;;,
      6;3; 0.000008, 2.857759, 0.000000;;,
      7;3; 0.000009, 2.857752, 0.000002;;,
      8;3; 0.000011, 2.857756, 0.000003;;,
      9;3; 0.000003, 2.857758, 0.000002;;,
      10;3; 0.000001, 2.857756,-0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.826624, 0.558883,-0.024203,-0.061298;;,
      1;4;-0.826624, 0.558883,-0.024203,-0.061298;;,
      2;4;-0.826624, 0.558883,-0.024203,-0.061298;;,
      3;4;-0.826624, 0.558883,-0.024203,-0.061298;;,
      4;4;-0.826624, 0.558883,-0.024203,-0.061298;;,
      5;4;-0.826624, 0.558883,-0.024203,-0.061298;;,
      6;4;-0.826624, 0.558882,-0.024203,-0.061298;;,
      7;4;-0.826624, 0.558882,-0.024203,-0.061298;;,
      8;4;-0.826624, 0.558882,-0.024202,-0.061298;;,
      9;4;-0.826624, 0.558882,-0.024203,-0.061298;;,
      10;4;-0.826624, 0.558882,-0.024202,-0.061298;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 0.999999, 1.000000;;,
      1;3; 1.000000, 0.999999, 1.000000;;,
      2;3; 1.000000, 0.999999, 1.000000;;,
      3;3; 1.000000, 0.999999, 1.000000;;,
      4;3; 1.000000, 0.999999, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 0.999999, 1.000000;;,
      7;3; 1.000000, 0.999999, 1.000000;;,
      8;3; 1.000000, 0.999999, 1.000000;;,
      9;3; 1.000000, 0.999999, 1.000000;;,
      10;3; 1.000000, 0.999999, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 1.265795,10.730978,-0.044079;;,
      1;3; 1.265800,10.730980,-0.044079;;,
      2;3; 1.265800,10.730972,-0.044080;;,
      3;3; 1.265783,10.730988,-0.044066;;,
      4;3; 1.265800,10.730986,-0.044072;;,
      5;3; 1.265789,10.730993,-0.044072;;,
      6;3; 1.265795,10.730986,-0.044069;;,
      7;3; 1.265799,10.730997,-0.044073;;,
      8;3; 1.265791,10.730990,-0.044074;;,
      9;3; 1.265797,10.730995,-0.044068;;,
      10;3; 1.265793,10.730997,-0.044063;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      1;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      2;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      3;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      4;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      5;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      6;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      7;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      8;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      9;4;-0.739543, 0.671190,-0.028401,-0.042102;;,
      10;4;-0.739543, 0.671190,-0.028401,-0.042102;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000001, 1.000000;;,
      1;3; 1.000000, 1.000001, 1.000000;;,
      2;3; 1.000000, 1.000001, 1.000000;;,
      3;3; 1.000000, 1.000001, 1.000000;;,
      4;3; 1.000000, 1.000001, 1.000000;;,
      5;3; 1.000000, 1.000001, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.001803, 4.302117, 0.000000;;,
      1;3; 0.001805, 4.302106, 0.000005;;,
      2;3; 0.001802, 4.302110, 0.000002;;,
      3;3; 0.001821, 4.302097, 0.000007;;,
      4;3; 0.001802, 4.302101, 0.000001;;,
      5;3; 0.001813, 4.302108, 0.000002;;,
      6;3; 0.001808, 4.302102, 0.000005;;,
      7;3; 0.001807, 4.302108, 0.000005;;,
      8;3; 0.001816, 4.302093,-0.000004;;,
      9;3; 0.001809, 4.302098, 0.000009;;,
      10;3; 0.001820, 4.302097, 0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      1;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      2;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      3;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      4;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      5;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      6;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      7;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      8;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      9;4;-0.959457, 0.281683,-0.004200, 0.008947;;,
      10;4;-0.959457, 0.281683,-0.004200, 0.008947;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000402, 4.040845, 0.000004;;,
      1;3; 0.000401, 4.040849, 0.000012;;,
      2;3; 0.000393, 4.040841,-0.000001;;,
      3;3; 0.000396, 4.040848, 0.000005;;,
      4;3; 0.000402, 4.040842, 0.000010;;,
      5;3; 0.000384, 4.040837, 0.000004;;,
      6;3; 0.000394, 4.040847, 0.000010;;,
      7;3; 0.000408, 4.040848, 0.000009;;,
      8;3; 0.000394, 4.040839, 0.000006;;,
      9;3; 0.000383, 4.040847, 0.000009;;,
      10;3; 0.000390, 4.040844, 0.000009;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      2;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      3;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      4;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      6;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      8;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      9;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      10;4;-1.000000, 0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.002202, 3.587903, 0.000034;;,
      1;3;-0.002197, 3.587904, 0.000019;;,
      2;3;-0.002201, 3.587905, 0.000018;;,
      3;3;-0.002200, 3.587904, 0.000017;;,
      4;3;-0.002192, 3.587908, 0.000018;;,
      5;3;-0.002193, 3.587905, 0.000016;;,
      6;3;-0.002187, 3.587910, 0.000009;;,
      7;3;-0.002197, 3.587908, 0.000007;;,
      8;3;-0.002194, 3.587907, 0.000009;;,
      9;3;-0.002179, 3.587908, 0.000008;;,
      10;3;-0.002185, 3.587913, 0.000009;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandMiddle4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      3;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      4;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      6;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      8;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      10;4;-1.000000, 0.000000, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000007, 3.587892,-0.000000;;,
      1;3; 0.000001, 3.587894,-0.000001;;,
      2;3;-0.000001, 3.587896, 0.000003;;,
      3;3;-0.000010, 3.587888, 0.000004;;,
      4;3; 0.000007, 3.587907,-0.000002;;,
      5;3;-0.000003, 3.587893,-0.000001;;,
      6;3; 0.000005, 3.587898, 0.000001;;,
      7;3; 0.000000, 3.587891, 0.000000;;,
      8;3; 0.000006, 3.587892, 0.000001;;,
      9;3; 0.000004, 3.587891,-0.000002;;,
      10;3;-0.000006, 3.587888, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      1;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      2;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      3;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      4;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      5;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      6;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      7;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      8;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      9;4;-0.771608, 0.635435,-0.020671,-0.020406;;,
      10;4;-0.771608, 0.635435,-0.020671,-0.020406;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 0.999999, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 0.999999, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 0.999999, 0.999999, 1.000000;;,
      8;3; 0.999999, 0.999999, 1.000000;;,
      9;3; 0.999999, 0.999999, 1.000000;;,
      10;3; 0.999999, 0.999999, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-1.563628,10.580308, 0.239527;;,
      1;3;-1.563628,10.580311, 0.239525;;,
      2;3;-1.563627,10.580309, 0.239531;;,
      3;3;-1.563641,10.580311, 0.239533;;,
      4;3;-1.563626,10.580312, 0.239530;;,
      5;3;-1.563637,10.580316, 0.239532;;,
      6;3;-1.563624,10.580311, 0.239535;;,
      7;3;-1.563626,10.580318, 0.239532;;,
      8;3;-1.563640,10.580322, 0.239530;;,
      9;3;-1.563629,10.580320, 0.239534;;,
      10;3;-1.563632,10.580319, 0.239539;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      1;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      2;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      3;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      4;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      5;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      6;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      7;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      8;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      9;4;-0.802517, 0.596125,-0.000826,-0.024520;;,
      10;4;-0.802517, 0.596125,-0.000826,-0.024520;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000541, 3.718162,-0.000014;;,
      1;3;-0.000537, 3.718167,-0.000013;;,
      2;3;-0.000545, 3.718158,-0.000012;;,
      3;3;-0.000540, 3.718152,-0.000008;;,
      4;3;-0.000546, 3.718158,-0.000006;;,
      5;3;-0.000539, 3.718166,-0.000008;;,
      6;3;-0.000545, 3.718157,-0.000008;;,
      7;3;-0.000542, 3.718163,-0.000003;;,
      8;3;-0.000535, 3.718163,-0.000004;;,
      9;3;-0.000549, 3.718160,-0.000004;;,
      10;3;-0.000531, 3.718163, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      1;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      2;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      3;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      4;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      5;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      6;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      7;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      8;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      9;4;-0.942166, 0.335113, 0.004006, 0.002654;;,
      10;4;-0.942166, 0.335113, 0.004006, 0.002654;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000319, 3.643931, 0.000004;;,
      1;3;-0.000317, 3.643940, 0.000025;;,
      2;3;-0.000308, 3.643938, 0.000015;;,
      3;3;-0.000309, 3.643931, 0.000020;;,
      4;3;-0.000304, 3.643936, 0.000017;;,
      5;3;-0.000304, 3.643933, 0.000023;;,
      6;3;-0.000304, 3.643930, 0.000020;;,
      7;3;-0.000300, 3.643933, 0.000022;;,
      8;3;-0.000306, 3.643928, 0.000020;;,
      9;3;-0.000302, 3.643929, 0.000020;;,
      10;3;-0.000302, 3.643932, 0.000023;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      4;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      5;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      6;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      7;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      8;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      9;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      10;4;-1.000000, 0.000000, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000001, 1.000000;;,
      2;3; 1.000000, 1.000001, 1.000000;;,
      3;3; 1.000000, 1.000001, 1.000000;;,
      4;3; 1.000000, 1.000001, 1.000000;;,
      5;3; 1.000000, 1.000001, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000001, 1.000000;;,
      8;3; 1.000000, 1.000001, 1.000000;;,
      9;3; 1.000000, 1.000001, 1.000000;;,
      10;3; 1.000000, 1.000001, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000875, 3.025716, 0.000008;;,
      1;3; 0.000874, 3.025711,-0.000006;;,
      2;3; 0.000874, 3.025722, 0.000004;;,
      3;3; 0.000859, 3.025706, 0.000000;;,
      4;3; 0.000866, 3.025709,-0.000006;;,
      5;3; 0.000874, 3.025698, 0.000002;;,
      6;3; 0.000863, 3.025706, 0.000006;;,
      7;3; 0.000870, 3.025709, 0.000000;;,
      8;3; 0.000862, 3.025703, 0.000000;;,
      9;3; 0.000866, 3.025702,-0.000005;;,
      10;3; 0.000871, 3.025709,-0.000004;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandRing4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      1;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      2;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      3;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      4;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000, 0.000000,-0.000000, 0.000000;;,
      6;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      7;4;-1.000000, 0.000000, 0.000000,-0.000000;;,
      8;4;-1.000000, 0.000000, 0.000000, 0.000000;;,
      9;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      10;4;-1.000000, 0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000001, 3.025676, 0.000010;;,
      1;3;-0.000002, 3.025679,-0.000000;;,
      2;3;-0.000003, 3.025685, 0.000006;;,
      3;3; 0.000007, 3.025700,-0.000002;;,
      4;3;-0.000007, 3.025680,-0.000002;;,
      5;3;-0.000010, 3.025678,-0.000002;;,
      6;3; 0.000002, 3.025684, 0.000004;;,
      7;3; 0.000012, 3.025682, 0.000002;;,
      8;3;-0.000004, 3.025682, 0.000004;;,
      9;3; 0.000001, 3.025679,-0.000004;;,
      10;3;-0.000001, 3.025683, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky1}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      1;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      2;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      3;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      4;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      5;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      6;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      7;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      8;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      9;4;-0.724283, 0.683921,-0.023950,-0.084215;;,
      10;4;-0.724283, 0.683921,-0.023950,-0.084215;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-3.578569,10.364507, 0.239114;;,
      1;3;-3.578563,10.364502, 0.239112;;,
      2;3;-3.578566,10.364502, 0.239114;;,
      3;3;-3.578577,10.364503, 0.239120;;,
      4;3;-3.578566,10.364507, 0.239123;;,
      5;3;-3.578573,10.364504, 0.239118;;,
      6;3;-3.578561,10.364505, 0.239120;;,
      7;3;-3.578571,10.364509, 0.239119;;,
      8;3;-3.578572,10.364506, 0.239114;;,
      9;3;-3.578567,10.364507, 0.239120;;,
      10;3;-3.578569,10.364506, 0.239122;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky2}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      1;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      2;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      3;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      4;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      5;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      6;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      7;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      8;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      9;4;-0.889368, 0.456902,-0.000120,-0.016295;;,
      10;4;-0.889368, 0.456902,-0.000120,-0.016295;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000001, 1.000000, 1.000000;;,
      1;3; 1.000001, 1.000000, 1.000000;;,
      2;3; 1.000001, 1.000000, 1.000000;;,
      3;3; 1.000001, 1.000000, 1.000000;;,
      4;3; 1.000001, 1.000000, 1.000000;;,
      5;3; 1.000001, 1.000000, 1.000000;;,
      6;3; 1.000001, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000001, 1.000000, 1.000000;;,
      9;3; 1.000001, 1.000000, 1.000000;;,
      10;3; 1.000001, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.001954, 3.211723, 0.000010;;,
      1;3;-0.001964, 3.211725, 0.000010;;,
      2;3;-0.001959, 3.211732, 0.000010;;,
      3;3;-0.001952, 3.211723, 0.000013;;,
      4;3;-0.001969, 3.211724, 0.000013;;,
      5;3;-0.001965, 3.211724, 0.000009;;,
      6;3;-0.001960, 3.211723, 0.000009;;,
      7;3;-0.001959, 3.211725, 0.000012;;,
      8;3;-0.001976, 3.211730, 0.000003;;,
      9;3;-0.001966, 3.211735, 0.000016;;,
      10;3;-0.001968, 3.211729, 0.000008;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky3}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      1;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      2;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      3;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      4;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      5;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      6;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      7;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      8;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      9;4;-0.974695, 0.223349,-0.000096,-0.009177;;,
      10;4;-0.974695, 0.223349,-0.000096,-0.009177;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.003105, 2.578161, 0.000031;;,
      1;3; 0.003103, 2.578157, 0.000022;;,
      2;3; 0.003098, 2.578163, 0.000014;;,
      3;3; 0.003100, 2.578150, 0.000021;;,
      4;3; 0.003098, 2.578150, 0.000024;;,
      5;3; 0.003100, 2.578156, 0.000022;;,
      6;3; 0.003105, 2.578155, 0.000021;;,
      7;3; 0.003110, 2.578148, 0.000021;;,
      8;3; 0.003092, 2.578153, 0.000027;;,
      9;3; 0.003082, 2.578154, 0.000021;;,
      10;3; 0.003090, 2.578152, 0.000022;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky4}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      1;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      2;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      3;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      4;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      5;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      6;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      7;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      8;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      9;4;-1.000000,-0.000001, 0.000000, 0.000000;;,
      10;4;-1.000000,-0.000001, 0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.001133, 2.342502,-0.000018;;,
      1;3;-0.001142, 2.342495,-0.000008;;,
      2;3;-0.001136, 2.342501,-0.000008;;,
      3;3;-0.001137, 2.342500,-0.000014;;,
      4;3;-0.001133, 2.342506,-0.000013;;,
      5;3;-0.001145, 2.342507,-0.000014;;,
      6;3;-0.001135, 2.342502,-0.000012;;,
      7;3;-0.001136, 2.342496,-0.000010;;,
      8;3;-0.001133, 2.342504,-0.000019;;,
      9;3;-0.001114, 2.342505,-0.000014;;,
      10;3;-0.001134, 2.342509,-0.000010;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightHandPinky4_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      1;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      2;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      3;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      4;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      5;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      6;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      7;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      8;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      9;4;-1.000000, 0.000000,-0.000000,-0.000000;;,
      10;4;-1.000000, 0.000000,-0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000001, 2.342495,-0.000001;;,
      1;3;-0.000002, 2.342492, 0.000004;;,
      2;3; 0.000009, 2.342495, 0.000005;;,
      3;3; 0.000002, 2.342498,-0.000003;;,
      4;3;-0.000002, 2.342496,-0.000003;;,
      5;3; 0.000002, 2.342505, 0.000006;;,
      6;3; 0.000000, 2.342498, 0.000005;;,
      7;3; 0.000007, 2.342496, 0.000000;;,
      8;3; 0.000000, 2.342497, 0.000003;;,
      9;3; 0.000006, 2.342495,-0.000004;;,
      10;3;-0.000003, 2.342495, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftUpLeg}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.043038, 0.180548, 0.403693, 0.895870;;,
      1;4;-0.053337, 0.189841, 0.368853, 0.908330;;,
      2;4;-0.052019, 0.198661, 0.345882, 0.915529;;,
      3;4;-0.047919, 0.203422, 0.348480, 0.913721;;,
      4;4;-0.041606, 0.200669, 0.356371, 0.911593;;,
      5;4;-0.028723, 0.188140, 0.374836, 0.907346;;,
      6;4;-0.021430, 0.165398, 0.407558, 0.897820;;,
      7;4;-0.013678, 0.141461, 0.439806, 0.886777;;,
      8;4;-0.000802, 0.125091, 0.457144, 0.880552;;,
      9;4; 0.016148, 0.111886, 0.467161, 0.876916;;,
      10;4; 0.034700, 0.099710, 0.474532, 0.873884;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 0.999991, 1.000000, 0.999999;;,
      1;3; 0.999991, 1.000000, 0.999999;;,
      2;3; 0.999991, 1.000000, 0.999999;;,
      3;3; 0.999991, 1.000000, 0.999998;;,
      4;3; 0.999991, 1.000000, 0.999999;;,
      5;3; 0.999991, 1.000000, 0.999999;;,
      6;3; 0.999990, 1.000000, 0.999999;;,
      7;3; 0.999990, 1.000000, 1.000000;;,
      8;3; 0.999990, 1.000000, 1.000000;;,
      9;3; 0.999990, 1.000000, 1.000000;;,
      10;3; 0.999990, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 7.707784,-5.105702, 0.126755;;,
      1;3; 7.707783,-5.105712, 0.126755;;,
      2;3; 7.707783,-5.105714, 0.126752;;,
      3;3; 7.707784,-5.105710, 0.126753;;,
      4;3; 7.707784,-5.105722, 0.126754;;,
      5;3; 7.707784,-5.105711, 0.126753;;,
      6;3; 7.707783,-5.105713, 0.126752;;,
      7;3; 7.707784,-5.105704, 0.126754;;,
      8;3; 7.707783,-5.105714, 0.126752;;,
      9;3; 7.707784,-5.105717, 0.126754;;,
      10;3; 7.707784,-5.105717, 0.126751;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftLeg}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.549605,-0.799392, 0.210573,-0.120690;;,
      1;4;-0.558696,-0.787873, 0.234284,-0.110572;;,
      2;4;-0.567154,-0.780775, 0.246466,-0.089333;;,
      3;4;-0.560421,-0.788163, 0.244470,-0.070451;;,
      4;4;-0.553967,-0.797955, 0.230047,-0.058882;;,
      5;4;-0.559398,-0.805367, 0.192521,-0.037317;;,
      6;4;-0.566679,-0.811875, 0.140393,-0.004844;;,
      7;4;-0.580471,-0.808604, 0.094471, 0.016982;;,
      8;4;-0.583711,-0.809343, 0.064367, 0.010176;;,
      9;4;-0.575316,-0.816736, 0.042584,-0.011829;;,
      10;4;-0.568709,-0.821456, 0.022650,-0.035585;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000,45.952511,-0.000003;;,
      1;3;-0.000000,45.952503, 0.000005;;,
      2;3; 0.000001,45.952507,-0.000007;;,
      3;3;-0.000001,45.952503, 0.000004;;,
      4;3; 0.000000,45.952503, 0.000002;;,
      5;3;-0.000000,45.952511, 0.000005;;,
      6;3; 0.000000,45.952499,-0.000004;;,
      7;3;-0.000000,45.952503,-0.000011;;,
      8;3; 0.000001,45.952496,-0.000001;;,
      9;3;-0.000001,45.952503, 0.000003;;,
      10;3; 0.000000,45.952507,-0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftFoot}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.941811, 0.333887, 0.005467, 0.038501;;,
      1;4;-0.936318, 0.347028, 0.011180, 0.052485;;,
      2;4;-0.929364, 0.360911, 0.021593, 0.074563;;,
      3;4;-0.913657, 0.388820, 0.039170, 0.111872;;,
      4;4;-0.885947, 0.433312, 0.056281, 0.155474;;,
      5;4;-0.858411, 0.473219, 0.071902, 0.184454;;,
      6;4;-0.829870, 0.518157, 0.077509, 0.191887;;,
      7;4;-0.808655, 0.552685, 0.074422, 0.187289;;,
      8;4;-0.787841, 0.583810, 0.071010, 0.182837;;,
      9;4;-0.770876, 0.608471, 0.067041, 0.176122;;,
      10;4;-0.758995, 0.625557, 0.064045, 0.168830;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 0.999999, 0.999999;;,
      1;3; 1.000000, 0.999999, 0.999999;;,
      2;3; 1.000000, 0.999999, 0.999999;;,
      3;3; 1.000000, 0.999999, 0.999999;;,
      4;3; 1.000000, 0.999999, 1.000000;;,
      5;3; 1.000000, 0.999999, 0.999999;;,
      6;3; 1.000000, 0.999999, 1.000000;;,
      7;3; 0.999999, 0.999999, 1.000000;;,
      8;3; 0.999999, 0.999999, 0.999999;;,
      9;3; 1.000000, 0.999999, 1.000000;;,
      10;3; 1.000000, 0.999999, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000002,44.565884,-0.000002;;,
      1;3;-0.000001,44.565880,-0.000004;;,
      2;3;-0.000002,44.565876,-0.000003;;,
      3;3; 0.000001,44.565880, 0.000002;;,
      4;3;-0.000002,44.565872,-0.000002;;,
      5;3; 0.000000,44.565872, 0.000003;;,
      6;3; 0.000001,44.565876, 0.000000;;,
      7;3; 0.000001,44.565872,-0.000001;;,
      8;3;-0.000001,44.565872, 0.000000;;,
      9;3; 0.000000,44.565872,-0.000003;;,
      10;3;-0.000001,44.565876,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftToeBase}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.969779, 0.144239,-0.169662, 0.099687;;,
      1;4;-0.968076, 0.150611,-0.173996, 0.099347;;,
      2;4;-0.965466, 0.180929,-0.165686, 0.087684;;,
      3;4;-0.961756, 0.205006,-0.162957, 0.080266;;,
      4;4;-0.959391, 0.181682,-0.193014, 0.096470;;,
      5;4;-0.953575, 0.198398,-0.205608, 0.095176;;,
      6;4;-0.953989, 0.202247,-0.200940, 0.092868;;,
      7;4;-0.954610, 0.208055,-0.193604, 0.089164;;,
      8;4;-0.950569, 0.228052,-0.193670, 0.083088;;,
      9;4;-0.943973, 0.254409,-0.196116, 0.075696;;,
      10;4;-0.936628, 0.280608,-0.198287, 0.068339;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000000,18.311058,-0.000001;;,
      1;3;-0.000001,18.311058,-0.000002;;,
      2;3; 0.000000,18.311060,-0.000004;;,
      3;3; 0.000000,18.311060,-0.000001;;,
      4;3;-0.000002,18.311058, 0.000001;;,
      5;3;-0.000002,18.311056,-0.000001;;,
      6;3;-0.000002,18.311056,-0.000002;;,
      7;3;-0.000004,18.311056,-0.000004;;,
      8;3;-0.000003,18.311058,-0.000002;;,
      9;3;-0.000000,18.311054,-0.000002;;,
      10;3;-0.000003,18.311054,-0.000003;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftToe_End}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      1;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      4;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000008, 6.608831,-0.000003;;,
      1;3; 0.000008, 6.608833,-0.000002;;,
      2;3; 0.000007, 6.608833,-0.000001;;,
      3;3; 0.000007, 6.608834,-0.000003;;,
      4;3; 0.000007, 6.608831,-0.000001;;,
      5;3; 0.000006, 6.608834,-0.000002;;,
      6;3; 0.000006, 6.608833,-0.000001;;,
      7;3; 0.000005, 6.608837,-0.000002;;,
      8;3; 0.000004, 6.608833,-0.000002;;,
      9;3; 0.000005, 6.608832,-0.000000;;,
      10;3; 0.000007, 6.608835,-0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_LeftToe_End_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      1;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      2;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      3;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      4;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      5;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      6;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      7;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      8;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      9;4;-1.000000,-0.000000, 0.000000,-0.000000;;,
      10;4;-1.000000,-0.000000, 0.000000,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000, 6.608831,-0.000001;;,
      1;3;-0.000000, 6.608831,-0.000001;;,
      2;3; 0.000000, 6.608830, 0.000003;;,
      3;3; 0.000000, 6.608830, 0.000001;;,
      4;3; 0.000000, 6.608830, 0.000002;;,
      5;3;-0.000002, 6.608830, 0.000001;;,
      6;3; 0.000001, 6.608830, 0.000000;;,
      7;3; 0.000004, 6.608830, 0.000000;;,
      8;3; 0.000000, 6.608830,-0.000000;;,
      9;3; 0.000002, 6.608829, 0.000000;;,
      10;3; 0.000001, 6.608832, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightUpLeg}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.100214,-0.175591, 0.310518, 0.928819;;,
      1;4;-0.085467,-0.156076, 0.260306, 0.948987;;,
      2;4;-0.072255,-0.130136, 0.210113, 0.966280;;,
      3;4;-0.065438,-0.082689, 0.189355, 0.976230;;,
      4;4;-0.065739,-0.026393, 0.176926, 0.981672;;,
      5;4;-0.064461, 0.003701, 0.220601, 0.973225;;,
      6;4;-0.068442, 0.007852, 0.250057, 0.965777;;,
      7;4;-0.071778, 0.007945, 0.239614, 0.968179;;,
      8;4;-0.073587, 0.005993, 0.217735, 0.973211;;,
      9;4;-0.075361, 0.003759, 0.193673, 0.978160;;,
      10;4;-0.075815, 0.000911, 0.166107, 0.983189;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 0.999967, 0.999997, 0.999997;;,
      1;3; 0.999965, 0.999998, 0.999998;;,
      2;3; 0.999964, 0.999999, 0.999999;;,
      3;3; 0.999962, 0.999999, 1.000000;;,
      4;3; 0.999962, 0.999999, 1.000000;;,
      5;3; 0.999962, 0.999999, 1.000000;;,
      6;3; 0.999962, 1.000000, 1.000000;;,
      7;3; 0.999962, 1.000000, 1.000000;;,
      8;3; 0.999962, 0.999999, 1.000000;;,
      9;3; 0.999962, 0.999999, 1.000000;;,
      10;3; 0.999962, 0.999999, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-7.707785,-5.105713, 0.340399;;,
      1;3;-7.707784,-5.105718, 0.340397;;,
      2;3;-7.707785,-5.105723, 0.340396;;,
      3;3;-7.707784,-5.105721, 0.340398;;,
      4;3;-7.707784,-5.105721, 0.340396;;,
      5;3;-7.707783,-5.105714, 0.340396;;,
      6;3;-7.707785,-5.105716, 0.340395;;,
      7;3;-7.707785,-5.105710, 0.340399;;,
      8;3;-7.707786,-5.105716, 0.340396;;,
      9;3;-7.707784,-5.105713, 0.340397;;,
      10;3;-7.707784,-5.105715, 0.340394;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightLeg}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.931445,-0.197311,-0.259997, 0.160875;;,
      1;4;-0.946802,-0.139335,-0.264966, 0.118086;;,
      2;4;-0.957777,-0.090083,-0.259596, 0.084605;;,
      3;4;-0.970973,-0.102577,-0.207528, 0.060179;;,
      4;4;-0.978742,-0.148158,-0.135860, 0.040699;;,
      5;4;-0.947443,-0.303791,-0.095341, 0.031190;;,
      6;4;-0.906301,-0.413580,-0.083497, 0.024467;;,
      7;4;-0.900757,-0.427153,-0.076855, 0.016446;;,
      8;4;-0.910041,-0.407538,-0.074696, 0.012557;;,
      9;4;-0.919460,-0.385572,-0.076137, 0.011403;;,
      10;4;-0.931441,-0.355167,-0.078477, 0.010721;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3;-0.000003,45.947220, 0.000002;;,
      1;3; 0.000000,45.947224, 0.000000;;,
      2;3; 0.000001,45.947216, 0.000000;;,
      3;3; 0.000001,45.947216, 0.000001;;,
      4;3; 0.000001,45.947227, 0.000002;;,
      5;3; 0.000000,45.947224, 0.000000;;,
      6;3;-0.000001,45.947224,-0.000000;;,
      7;3;-0.000000,45.947231, 0.000001;;,
      8;3; 0.000000,45.947224,-0.000000;;,
      9;3; 0.000001,45.947216, 0.000000;;,
      10;3; 0.000001,45.947220, 0.000000;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightFoot}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.983206, 0.177278, 0.039118,-0.018687;;,
      1;4;-0.984607, 0.168844, 0.044601,-0.007113;;,
      2;4;-0.981870, 0.184924, 0.041434,-0.004340;;,
      3;4;-0.960779, 0.275755, 0.025671,-0.014282;;,
      4;4;-0.911955, 0.409265, 0.017488,-0.023121;;,
      5;4;-0.825433, 0.563098, 0.014629,-0.036979;;,
      6;4;-0.756534, 0.652969, 0.020161,-0.029679;;,
      7;4;-0.717159, 0.696107, 0.026519,-0.020372;;,
      8;4;-0.701362, 0.711989, 0.030871,-0.014530;;,
      9;4;-0.697332, 0.715882, 0.033158,-0.011914;;,
      10;4;-0.699949, 0.713279, 0.034699,-0.009999;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 0.999999, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000001,44.572094,-0.000000;;,
      1;3;-0.000001,44.572098,-0.000001;;,
      2;3; 0.000000,44.572098, 0.000001;;,
      3;3; 0.000001,44.572094,-0.000000;;,
      4;3; 0.000000,44.572098,-0.000000;;,
      5;3;-0.000000,44.572094,-0.000000;;,
      6;3; 0.000002,44.572090,-0.000001;;,
      7;3; 0.000000,44.572102, 0.000000;;,
      8;3; 0.000000,44.572102, 0.000001;;,
      9;3; 0.000000,44.572098, 0.000002;;,
      10;3; 0.000002,44.572094, 0.000001;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightToeBase}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.916806, 0.383816, 0.097952,-0.050560;;,
      1;4;-0.909171, 0.399549, 0.100556,-0.060473;;,
      2;4;-0.904815, 0.407018, 0.105707,-0.066881;;,
      3;4;-0.907120, 0.398207, 0.117931,-0.068245;;,
      4;4;-0.913085, 0.379166, 0.134081,-0.067309;;,
      5;4;-0.916228, 0.368606, 0.142160,-0.066675;;,
      6;4;-0.919188, 0.361602, 0.141418,-0.065870;;,
      7;4;-0.922341, 0.353957, 0.140631,-0.064996;;,
      8;4;-0.921451, 0.356135, 0.140853,-0.065245;;,
      9;4;-0.918862, 0.362382, 0.141500,-0.065960;;,
      10;4;-0.916228, 0.368606, 0.142161,-0.066676;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 0.999999, 1.000000;;,
      1;3; 1.000000, 0.999999, 1.000000;;,
      2;3; 1.000000, 0.999999, 1.000000;;,
      3;3; 1.000000, 0.999999, 1.000000;;,
      4;3; 1.000000, 0.999999, 1.000000;;,
      5;3; 1.000000, 1.000000, 0.999999;;,
      6;3; 1.000000, 0.999999, 0.999999;;,
      7;3; 1.000000, 0.999999, 0.999999;;,
      8;3; 1.000000, 0.999999, 1.000000;;,
      9;3; 1.000000, 0.999999, 1.000000;;,
      10;3; 1.000000, 0.999999, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000,18.116955,-0.000011;;,
      1;3; 0.000002,18.116953,-0.000008;;,
      2;3; 0.000002,18.116951,-0.000010;;,
      3;3; 0.000001,18.116953,-0.000009;;,
      4;3; 0.000002,18.116951,-0.000010;;,
      5;3; 0.000001,18.116951,-0.000009;;,
      6;3; 0.000000,18.116951,-0.000009;;,
      7;3; 0.000002,18.116949,-0.000009;;,
      8;3; 0.000001,18.116951,-0.000010;;,
      9;3; 0.000000,18.116949,-0.000009;;,
      10;3;-0.000000,18.116953,-0.000006;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightToe_End}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      1;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      2;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      3;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      4;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      5;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      6;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      7;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      8;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      9;4;-1.000000, 0.000000, 0.000004,-0.000000;;,
      10;4;-1.000000, 0.000000, 0.000004,-0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000001;;,
      1;3; 1.000000, 1.000000, 1.000001;;,
      2;3; 1.000000, 1.000000, 1.000001;;,
      3;3; 1.000000, 1.000000, 1.000001;;,
      4;3; 1.000000, 1.000001, 1.000001;;,
      5;3; 0.999999, 1.000000, 1.000001;;,
      6;3; 1.000000, 1.000000, 1.000001;;,
      7;3; 1.000000, 1.000000, 1.000001;;,
      8;3; 1.000000, 1.000000, 1.000001;;,
      9;3; 1.000000, 1.000000, 1.000001;;,
      10;3; 1.000000, 1.000000, 1.000001;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000003, 6.707265, 0.000001;;,
      1;3; 0.000001, 6.707264, 0.000000;;,
      2;3; 0.000000, 6.707263, 0.000001;;,
      3;3; 0.000003, 6.707266, 0.000001;;,
      4;3; 0.000000, 6.707267,-0.000000;;,
      5;3; 0.000000, 6.707265, 0.000001;;,
      6;3;-0.000000, 6.707266, 0.000000;;,
      7;3;-0.000001, 6.707267,-0.000000;;,
      8;3; 0.000001, 6.707267, 0.000001;;,
      9;3;-0.000001, 6.707267, 0.000000;;,
      10;3;-0.000002, 6.707265, 0.000002;;;
    }
  }
  Animation {
    {Armature_mixamorig_RightToe_End_end}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      1;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      2;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      3;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      4;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      5;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      6;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      7;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      8;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      9;4;-1.000000,-0.000000,-0.000000, 0.000000;;,
      10;4;-1.000000,-0.000000,-0.000000, 0.000000;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3; 1.000000, 1.000000, 1.000000;;,
      1;3; 1.000000, 1.000000, 1.000000;;,
      2;3; 1.000000, 1.000000, 1.000000;;,
      3;3; 1.000000, 1.000000, 1.000000;;,
      4;3; 1.000000, 1.000000, 1.000000;;,
      5;3; 1.000000, 1.000000, 1.000000;;,
      6;3; 1.000000, 1.000000, 1.000000;;,
      7;3; 1.000000, 1.000000, 1.000000;;,
      8;3; 1.000000, 1.000000, 1.000000;;,
      9;3; 1.000000, 1.000000, 1.000000;;,
      10;3; 1.000000, 1.000000, 1.000000;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000000, 6.707255, 0.000002;;,
      1;3; 0.000001, 6.707251,-0.000001;;,
      2;3;-0.000002, 6.707252, 0.000001;;,
      3;3;-0.000000, 6.707255, 0.000000;;,
      4;3;-0.000001, 6.707253, 0.000002;;,
      5;3; 0.000000, 6.707253,-0.000000;;,
      6;3;-0.000000, 6.707255, 0.000000;;,
      7;3;-0.000000, 6.707254,-0.000000;;,
      8;3; 0.000000, 6.707254,-0.000000;;,
      9;3;-0.000000, 6.707254,-0.000000;;,
      10;3;-0.000001, 6.707255,-0.000000;;;
    }
  }
  Animation {
    {Backpack}
    AnimationKey { // Rotation
      0;
      11;
      0;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      1;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      2;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      3;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      4;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      5;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      6;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      7;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      8;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      9;4;-0.000000,-0.000000, 0.707107, 0.707107;;,
      10;4;-0.000000,-0.000000, 0.707107, 0.707107;;;
    }
    AnimationKey { // Scale
      1;
      11;
      0;3;26.076880,26.076878,26.076880;;,
      1;3;26.076880,26.076878,26.076880;;,
      2;3;26.076880,26.076878,26.076880;;,
      3;3;26.076880,26.076878,26.076880;;,
      4;3;26.076880,26.076878,26.076880;;,
      5;3;26.076880,26.076878,26.076880;;,
      6;3;26.076880,26.076878,26.076880;;,
      7;3;26.076880,26.076878,26.076880;;,
      8;3;26.076880,26.076878,26.076880;;,
      9;3;26.076880,26.076878,26.076880;;,
      10;3;26.076880,26.076878,26.076880;;;
    }
    AnimationKey { // Position
      2;
      11;
      0;3; 0.000007,137.266464,-11.664410;;,
      1;3; 0.000007,137.266464,-11.664410;;,
      2;3; 0.000007,137.266464,-11.664410;;,
      3;3; 0.000007,137.266464,-11.664410;;,
      4;3; 0.000007,137.266464,-11.664410;;,
      5;3; 0.000007,137.266464,-11.664410;;,
      6;3; 0.000007,137.266464,-11.664410;;,
      7;3; 0.000007,137.266464,-11.664410;;,
      8;3; 0.000007,137.266464,-11.664410;;,
      9;3; 0.000007,137.266464,-11.664410;;,
      10;3; 0.000007,137.266464,-11.664410;;;
    }
  }
} // End of AnimationSet Global
