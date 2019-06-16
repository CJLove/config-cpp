pipeline {
    agent none

	parameters {
        // Build on Fedora's default compiler as first sequential stage
        booleanParam name: 'Use_gcc8', defaultValue: true, description: 'Build/test using gcc8'
        booleanParam name: 'Use_clang7', defaultValue: true, description: 'Build/test using clang7'
        // Sanitizer tests in parallel stages
        booleanParam name: 'Run_sanitizers', defaultValue: true, description: 'Build/test using sanitizers'
        // Build alternative compilers in parallel stages
        booleanParam name: 'Use_gcc6', defaultValue: true, description: 'Build/test using gcc6'
        booleanParam name: 'Use_gcc7', defaultValue: true, description: 'Build/test using gcc7'
        booleanParam name: 'Use_gcc9', defaultValue: true, description: 'Build/test using gcc9'
	}
    stages {
        stage('Parallel system compiler stages') {
            failFast true
            parallel {
                stage('gcc8.3.1') {
                    when {
                        environment name: 'Use_gcc8', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} using gcc 8.3.1"
                        dir ("gcc831") {
                            sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc831/unittests.xml'
                        }
                    }
                }
                stage('clang7') {
                    when {
                        environment name: 'Use_gcc8', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} using clang 7"
                        dir ("clang7") {
                            sh 'CC=clang CXX=clang++ cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }
                
                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'clang/unittests.xml'
                        }
                    }
                }
            }
        }

        stage('Parallel sanitizer stages') {
            failFast true
            parallel {
                stage('asan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc831:latest"
                            args "--cap-add SYS_PTRACE"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("asan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=asan -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }

                stage('tsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("tsan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=tsan -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }

                stage('ubsan') {
                    when {
                        environment name: 'Run_sanitizers', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc831:latest"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} with asan"
                        dir ("ubsan") {
                            sh 'cmake -DCMAKE_BUILD_TYPE=ubsan -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                    }
                }
            }
        }

        stage('Parallel alternate compiler stages') {
            failFast true
            parallel {
                stage('gcc9.1.0') {
                    when {
                        environment name: 'Use_gcc9', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc910:latest"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} using gcc 9.1.0"
                        dir ("gcc910") {
                            sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc910/unittests.xml'
                        }
                    }
                }

                stage('gcc7.4.0') {
                    when {
                        environment name: 'Use_gcc7', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc740:latest"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} using gcc 7.4.0"
                        dir ("gcc740") {
                            sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc740/unittests.xml'
                        }
                    }
                }

                stage('gcc6.5.0') {
                    when {
                        environment name: 'Use_gcc6', value: 'true'
                    }
                    agent {
                        docker {
                            label 'fir'
                            image "config-cpp-gcc650:latest"
                        }
                    }
                    steps {
                        echo "building config-cpp branch ${env.BRANCH_NAME} using gcc 6.5.0"
                        dir ("gcc650") {
                            sh 'cmake -DYAML_SUPPORT=ON -DJSON_SUPPORT=ON -DBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=OFF -DCMAKE_INSTALL_PREFIX=$PWD/.. -DBUILD_TESTS=ON ..'
                            sh 'make'
                            sh "./test/ConfigCppTests --gtest_output=xml:unittests.xml"
                        }
                
                    }

                    post {
                        always {
                            junit allowEmptyResults: true, testResults: 'gcc650/unittests.xml'
                        }
                    }
                }
            }
        }
    }
    
}