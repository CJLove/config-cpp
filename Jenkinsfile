pipeline {
    agent none

	parameters {
        booleanParam name: 'Use_gcc6', defaultValue: false, description: 'Build/test using gcc6'
        booleanParam name: 'Use_gcc7', defaultValue: false, description: 'Build/test using gcc7'
        booleanParam name: 'Use_gcc8', defaultValue: true, description: 'Build/test using gcc8'
        booleanParam name: 'Use_gcc9', defaultValue: false, description: 'Build/test using gcc9'
        string name: 'IMAGE', defaultValue: 'config-cpp-gcc831', description: 'Docker image'
	}
    stages {
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
                    sh 'ctest -T --no-compress-output'
                }
                
            }
        }
    }
    post {
        always {
            // Archive the CTest xml output
            archiveArtifacts (
                artifacts: '*/Testing/**/*.xml',
                fingerprint: true
            )

            // Process the CTest xml output with the xUnit plugin
            xunit (
                testTimeMargin: '3000',
                thresholdMode: 1,
                thresholds: [
                skipped(failureThreshold: '0'),
                failed(failureThreshold: '0')
                ],
                tools: [CTest(
                pattern: '*/Testing/**/*.xml',
                deleteOutputFiles: true,
                failIfNotNew: false,
                skipNoTestFiles: true,
                stopProcessingIfError: true
                )]
            )
        }
    }

    
}