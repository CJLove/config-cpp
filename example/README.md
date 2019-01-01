# Example programs

- `json` - application supporting a JSON configuration file, including unmarshalling config data into a native struct.
- `yaml` - application supporting a YAML configuration file, including unmarshalling config data into a native struct.
- `watcher` - application supporting a JSON configuration file and registering for configuration file updates.

## Kubernetes deployment
The `watcher` application can be deployed in a Kubernetes pod with the configuration supplied via ConfigMap.  The application will then handle updates to the ConfigMap.  See Dockerfiles and manifests under the `docker` directory.